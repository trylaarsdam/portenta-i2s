#pragma once
#include "i2s_init.h"
#include "Arduino.h"
#ifndef PI
#define PI 3.14159265358979323846
#endif
#define TAU (2.0 * PI)
#define FREQ_OFFSET 5

#define USE_I2S1 false
#define USE_I2S2 true

#define I2S_BUFFER_SIZE 8192U

/**
 * @brief Portenta I2S class
*/
class PortentaI2S 
{
	private:
		bool i2sSelection;
		unsigned long audioFreq;
		GPIO_InitTypeDef GPIO_InitStruct;
		void I2S_Init(void);
		void GPIO_Init(void);
		uint16_t audioBuffer[I2S_BUFFER_SIZE]; // for both left + right
		const int nsamples = I2S_BUFFER_SIZE / 2;
	public:
		PortentaI2S(bool i2s = USE_I2S2, uint32_t sampleRate = 44100.0);
		void begin(void);
		void play(uint16_t* buffer, int bufferSize);
};