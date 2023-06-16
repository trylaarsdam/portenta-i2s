#include "portenta-i2s.h"

void PortentaI2S::I2S_Init(void)
{
	switch (i2sSelection)
	{
	case USE_I2S1:
		MX_I2S1_Init(audioFreq);
		break;
	case USE_I2S2:
		MX_I2S2_Init(audioFreq);
		break;
	}
}

void PortentaI2S::GPIO_Init(void)
{
	switch (i2sSelection)
	{
	case USE_I2S1:
		MX_GPIO_Init_I2S1();
		break;
	case USE_I2S2:
		MX_GPIO_Init_I2S2();
		break;
	}
}

PortentaI2S::PortentaI2S(bool i2s, uint32_t sampleRate)
{
	i2sSelection = i2s;
	audioFreq = sampleRate;
}

void PortentaI2S::begin(void)
{
	GPIO_Init();
	I2S_Init();
}

void PortentaI2S::play(uint16_t* buffer, int bufferSize)
{
	/*
	if(HAL_I2S_Transmit(&hi2s2, buffer, bufferSize, HAL_MAX_DELAY) != HAL_OK)
	{
		Serial.println("HAL I2S Transmit Error");
	}
	delay(bufferSize / audioFreq * 1000);
	*/
	
	uint16_t numBuffers = bufferSize / I2S_BUFFER_SIZE;
	if(bufferSize % I2S_BUFFER_SIZE != 0)
	{
		numBuffers++;
	}

	for(int i = 0; i < numBuffers; i++)
	{
		// determine end of the slice
		uint32_t endBufferPosition = numBuffers * I2S_BUFFER_SIZE;
		if(endBufferPosition > bufferSize)
		{
			endBufferPosition = bufferSize;
		}
		// copy slice into audioBuffer
		for(uint32_t j = (i * I2S_BUFFER_SIZE); j < endBufferPosition; j++)
		{
			audioBuffer[j - (i * I2S_BUFFER_SIZE)] = buffer[j];
		}

		// transmit audioBuffer
		if(i2sSelection == USE_I2S1)
		{
			// HAL_I2S_Transmit(&hi2s1, audioBuffer, I2S_BUFFER_SIZE, HAL_MAX_DELAY);
		}
		else
		{
			HAL_I2S_Transmit(&hi2s2, audioBuffer, I2S_BUFFER_SIZE, HAL_MAX_DELAY);
		}
		delay(I2S_BUFFER_SIZE / audioFreq * 1000);
	}
}