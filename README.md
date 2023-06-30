# Arduino Portenta H7 I2S Library

This library allows you to use the I2S protocol on the Arduino Portenta H7 in PlatformIO. It is compatible with both the M4 and M7 cores on the Portenta.

Huge thanks to Max Gerhardt from PlatformIO for helping me get I2S ported from Cube to PlatformIO.

## Usage
1. Create the I2S object
```cpp
PortentaI2S i2s = PortentaI2S(USE_I2S2, I2S_AUDIOFREQ_44K);
```

Currently only I2S2 is supported on the Portenta (this is connected to the I2S header on the Portenta breakout board). However, you can chose any audio frequency you wish from this list:
```cpp
I2S_AUDIOFREQ_192K               192000hz
I2S_AUDIOFREQ_96K                96000hz
I2S_AUDIOFREQ_48K                48000hz
I2S_AUDIOFREQ_44K                44100hz 
I2S_AUDIOFREQ_32K                32000hz
I2S_AUDIOFREQ_22K                22050hz
I2S_AUDIOFREQ_16K                16000hz
I2S_AUDIOFREQ_11K                11025hz
I2S_AUDIOFREQ_8K                 8000hz
I2S_AUDIOFREQ_DEFAULT            2hz
```

2. Initialize the I2S interface
```cpp
i2s.begin();
```

3. Play your audio buffer

You can do this 2 ways - either by create a 16bit audio buffer and calling play:
```cpp
i2s.play(buffer, bufferLen);
```

Or you can also read a raw 8-bit PCM audio file, and provide it directly to i2s.play:
```cpp
#include "audio-file.h"
i2s.play(audio_file_raw, audio_file_raw_len);
```

You can see documentation on how to generate such a file [here](https://dev.toddr.org/i2s-audio-playback-on-the-portenta-h7/).

4. Record audio into a 32bit buffer
```cpp
#define BUFFER_LEN 8000 * 5 * 2 // 8000hz for 5 seconds for 2 channels (left, right)
uint32_t rxBuffer[BUFFER_LEN];
i2s.record(rxBuffer, BUFFER_LEN);
```

## Examples
Two examples are provided - one which generates and plays back a simple sine wave, and one which plays back a prerecorded audio file, records audio from the microphone immediately after, and then sends the recorded audio to the serial port, where you can parse it with Audacity as 32-bit signed PCM stereo audio. You can find a full guide [here](https://dev.toddr.org/i2s-audio-playback-on-the-portenta-h7/).