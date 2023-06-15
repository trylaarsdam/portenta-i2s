#ifdef CORE_CM4

#include <Arduino.h>


// #ifndef PI
// #define PI 3.14159265358979323846
// #endif
// #define TAU (2.0 * PI)
// #define FREQ_OFFSET 5

// void setup()
// {
//     MX_GPIO_Init();
//     MX_I2S2_Init();
//     // fill signal buffer with sine wave
//     int i = 0;
//     while(i < nsamples) {
//         double t = ((double)i/2.0)/((double)nsamples);
//         samplebuf[i] = 32767*sin(100.0 * TAU * t); // left channel
//         samplebuf[i+1] = samplebuf[i]; // right channel (same)
//         i += 2;
//     }
// }

void setup()
{
	// const double sampleRate = 44100.0; // substitute with your actual sample rate
	// MX_GPIO_Init_I2S2();
	// MX_I2S2_Init(I2S_AUDIOFREQ_44K);
	// // fill signal buffer with sine wave

}


void loop()
{
    // transmit full buffer over and over again
    // HAL_I2S_Transmit(&hi2s2, samplebuf, nsamples, HAL_MAX_DELAY);
		// delay(500);
}

#endif

#ifdef CORE_CM7
#include "Arduino.h"
#include <portenta-i2s.h>

PortentaI2S i2s = PortentaI2S(USE_I2S2, I2S_AUDIOFREQ_44K);

void setup()
{
	i2s.begin();
	uint16_t samplebuf[8192]; // for both left + right
	const int nsamples = sizeof(samplebuf) / sizeof(samplebuf[0]);
	int i = 0;
	while(i < nsamples) {
		double t = ((double)i / 2.0) / 44100.0;
		samplebuf[i] = 32767*((sin((440.0 - FREQ_OFFSET) * TAU * t) / 2)); // left channel
		samplebuf[i+1] = samplebuf[i]; // right channel (same)
		i += 2;
	}
	while(1){
		HAL_I2S_Transmit(&hi2s2, samplebuf, nsamples, HAL_MAX_DELAY);
	}
}

void loop()
{
	
}
#endif