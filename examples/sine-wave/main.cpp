#ifdef CORE_CM4

#include <Arduino.h>
#include <portenta-i2s.h>

PortentaI2S i2s = PortentaI2S(USE_I2S2, I2S_AUDIOFREQ_44K);

void setup()
{
	i2s.begin();

	// Generate sine wave
	uint16_t samplebuf[8192]; // for both left + right
	const int nsamples = sizeof(samplebuf) / sizeof(samplebuf[0]);
	int i = 0;
	while(i < nsamples) {
		double t = ((double)i / 2.0) / 44100.0;
		samplebuf[i] = 32767*((sin((440.0 - FREQ_OFFSET) * TAU * t) / 2)); // left channel
		samplebuf[i+1] = samplebuf[i]; // right channel (same)
		i += 2;
	}

	// Play continuously
	while(1){
		i2s.play(samplebuf, nsamples);
	}
}

void loop()
{
	
}

#endif

#ifdef CORE_CM7
#include "Arduino.h"
#include <portenta-i2s.h>

PortentaI2S i2s = PortentaI2S(USE_I2S2, I2S_AUDIOFREQ_44K);

void setup()
{
	Serial.begin(115200);
	i2s.begin();

	// Generate sine wave
	uint16_t samplebuf[8192]; // for both left + right
	const int nsamples = sizeof(samplebuf) / sizeof(samplebuf[0]);
	int i = 0;
	while(i < nsamples) {
		double t = ((double)i / 2.0) / 44100.0;
		samplebuf[i] = 32767*((sin((440.0 - FREQ_OFFSET) * TAU * t) / 2)); // left channel
		samplebuf[i+1] = samplebuf[i]; // right channel (same)
		i += 2;
	}

	// Play continuously
	while(1){
		i2s.play(samplebuf, nsamples);
	}
}

void loop()
{
	
}
#endif