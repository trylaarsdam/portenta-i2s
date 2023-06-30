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
	// I2S_Interrupt_Init();
	// I2S2_IRQHandler();
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

void PortentaI2S::play(uint8_t* file, unsigned int fileSize)
{
	I2S2_Prep_TX();
	uint16_t buffer[8192];
	memset(buffer, 0, (8192) * sizeof(uint16_t));
	for(unsigned int i = 0; i < fileSize; i++)
	{
		// convert file bytes to 32 bit samples
		uint8_t unsigned8Bit = file[i];
		buffer[(i * 2) % 8192] = (uint16_t)unsigned8Bit << 8; // left channel
		buffer[(i * 2 + 1) % 8192] = (uint16_t)unsigned8Bit << 8; // right channel

		// transmit buffer if full (4096 samples)
		if(i % 4096 == 0 && i != 0)
		{
			HAL_I2S_Transmit(&hi2s2, buffer, 8192, HAL_MAX_DELAY);
			memset(buffer, 0, (8192) * sizeof(uint16_t));
		}
		else if(i == fileSize - 1)
		{
			HAL_I2S_Transmit(&hi2s2, buffer, 8192, HAL_MAX_DELAY);
		}
	}

	delete[] buffer;
	I2S2_Prep_RX();
}

void PortentaI2S::record(uint32_t* buffer, unsigned int bufferSize)
{
	HAL_I2S_Transmit(&hi2s2, (uint16_t*)buffer, 1, HAL_MAX_DELAY);
	uint32_t counter = bufferSize, dummy;
	// Check if the Peripheral is enabled
	if ((hi2s2.Instance->CR1 & SPI_CR1_SPE) == SPI_CR1_SPE)
	{
		/* Disable the I2S peripheral */
		__HAL_I2S_DISABLE(&hi2s2);
	}
	
	// Clear all flags
	__HAL_I2S_CLEAR_OVRFLAG(&hi2s2);
	__HAL_I2S_CLEAR_TIFREFLAG(&hi2s2);
	__HAL_I2S_CLEAR_UDRFLAG(&hi2s2);
	
	//Check if there are dummy data in the RXDR
	if(__HAL_I2S_GET_FLAG(&hi2s2, I2S_FLAG_RXP) == SET)
	{
		dummy = hi2s2.Instance->RXDR;
	}
	
	/* Enable I2S peripheral */
	__HAL_I2S_ENABLE(&hi2s2);
	
	/* Start the transfer */
	SET_BIT(hi2s2.Instance->CR1, SPI_CR1_CSTART);
 
	while(counter > 0)
	{
		while (__HAL_I2S_GET_FLAG(&hi2s2, I2S_FLAG_RXP) != SET);
 
		buffer[bufferSize - counter] = *((uint32_t *)&hi2s2.Instance->RXDR);
 
		if (__HAL_I2S_GET_FLAG(&hi2s2, I2S_FLAG_OVR) == SET)
		{
			/* Clear overrun flag */
			__HAL_I2S_CLEAR_OVRFLAG(&hi2s2);
			// Serial.println("OVR");
		}
 
		counter--;
	}
	
	// Disable the peripheral
	__HAL_I2S_DISABLE(&hi2s2);
}