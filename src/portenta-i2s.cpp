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

void PortentaI2S::record(uint32_t* buffer, uint32_t size)
{
	uint32_t counter = size, dummy;

	// Check if I2S is enabled
	if((hi2s2.Instance->CR1 & SPI_CR1_SPE) == SPI_CR1_SPE)
	{
		// Disable it
		__HAL_I2S_DISABLE(&hi2s2);
	}

	// Clear I2S flags
	__HAL_I2S_CLEAR_OVRFLAG(&hi2s2);
	__HAL_I2S_CLEAR_TIFREFLAG(&hi2s2);
	__HAL_I2S_CLEAR_UDRFLAG(&hi2s2);

	// Check for dummy data in RXDR
	if(__HAL_I2S_GET_FLAG(&hi2s2, I2S_FLAG_RXP) == SET)
	{
		dummy = hi2s2.Instance->RXDR;
	}

	// Enable I2S
	__HAL_I2S_ENABLE(&hi2s2);

	// Start data transfer
	SET_BIT(hi2s2.Instance->CR1, SPI_CR1_CSTART);

	while(counter > 0)
	{
		// wait for RXP flag to be set
		while(__HAL_I2S_GET_FLAG(&hi2s2, I2S_FLAG_RXP) != SET);

		buffer[size - counter] = *((uint32_t *)&hi2s2.Instance->RXDR);

		// check for overruns
		if(__HAL_I2S_GET_FLAG(&hi2s2, I2S_FLAG_OVR) == SET)
		{
			// clear overrun flag
			__HAL_I2S_CLEAR_OVRFLAG(&hi2s2);
		}

		counter--;
	}

	__HAL_I2S_DISABLE(&hi2s2);
}

void PortentaI2S::record(uint16_t* buffer, uint32_t size)
{
	uint32_t counter = size, dummy;

	// Check if I2S is enabled
	if((hi2s2.Instance->CR1 & SPI_CR1_SPE) == SPI_CR1_SPE)
	{
		// Disable it
		__HAL_I2S_DISABLE(&hi2s2);
	}

	// Clear I2S flags
	__HAL_I2S_CLEAR_OVRFLAG(&hi2s2);
	__HAL_I2S_CLEAR_TIFREFLAG(&hi2s2);
	__HAL_I2S_CLEAR_UDRFLAG(&hi2s2);

	// Check for dummy data in RXDR
	if(__HAL_I2S_GET_FLAG(&hi2s2, I2S_FLAG_RXP) == SET)
	{
		dummy = hi2s2.Instance->RXDR;
	}

	// Enable I2S
	__HAL_I2S_ENABLE(&hi2s2);

	// Start data transfer
	SET_BIT(hi2s2.Instance->CR1, SPI_CR1_CSTART);

	while(counter > 0)
	{
		// wait for RXP flag to be set
		while(__HAL_I2S_GET_FLAG(&hi2s2, I2S_FLAG_RXP) != SET);

		buffer[size - counter] = *((uint16_t *)&hi2s2.Instance->RXDR);

		// check for overruns
		if(__HAL_I2S_GET_FLAG(&hi2s2, I2S_FLAG_OVR) == SET)
		{
			// clear overrun flag
			__HAL_I2S_CLEAR_OVRFLAG(&hi2s2);
		}

		counter--;
	}

	__HAL_I2S_DISABLE(&hi2s2);
}