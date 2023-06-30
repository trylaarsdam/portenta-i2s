#include "Arduino.h"
#include <portenta-i2s.h>
#include "ARTiE_Fork.h"

#define I2S_BUFFER_SIZE I2S_AUDIOFREQ_8K * 5 * 2 // 5 seconds of audio, 2 channels

PortentaI2S i2s = PortentaI2S(USE_I2S2, I2S_AUDIOFREQ_8K);

uint32_t myBuffer[I2S_BUFFER_SIZE]; // RX buffer

void setup()
{
	Serial.begin(115200); // Begin serial communication to send file
	i2s.begin(); // Initialize I2S

	i2s.play(ARTiE_Fork_raw, ARTiE_Fork_raw_len); // Play the 8bit signed audio file
	i2s.record(myBuffer, I2S_BUFFER_SIZE); // Record 32bit signed audio into myBuffer for 5 seconds

	// Print the recorded audio to serial
	for(int i = 0; i < I2S_BUFFER_SIZE; i++)
	{
		if(i % 1 == 0) // 2 words per sample, 2 channels of data
		{
			volatile uint32_t sample = myBuffer[i];
			sample = sample >> 14;
			int32_t signedSample = sample; // center at 0
			signedSample = sample * 65534; // scale to 32bit from 16bit
			Serial.write((uint8_t*)&signedSample, sizeof(int32_t));
		}
	}
}

void loop()
{
	// do nothing
}