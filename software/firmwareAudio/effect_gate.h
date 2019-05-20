/* Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef effect_gate_h_
#define effect_gate_h_

#include "Arduino.h"
#include "AudioStream.h"

const int S_Floor = 0;
const int S_Attack = 1;
const int S_Hold = 2;
const int S_Release = 3;

class AudioEffectGate : public AudioStream
{
public:
	AudioEffectGate(void)
	  : AudioStream(1, inputQueueArray) {
		  currentGain = 0.0;
		  thresholdLevelInt = 16384;	//0.5
		  attackTimeDelta = 1.0f / (50.0f / (1000.0f / (AUDIO_SAMPLE_RATE / AUDIO_BLOCK_SAMPLES)));			//50ms default
		  holdTimeDelta = 1.0f / (200.0f / (1000.0f / (AUDIO_SAMPLE_RATE / AUDIO_BLOCK_SAMPLES)));			//200ms default
		  currentHoldTime = 0.0f;
		  releaseTimeDelta = 1.0f / (200.0f / (1000.0f / (AUDIO_SAMPLE_RATE / AUDIO_BLOCK_SAMPLES)));		//2000ms default
		  floorGain = 0.0f;
		  state = S_Floor;
	  }
	void threshold(float threshold) {
		if (threshold > 1.0f) threshold = 1.0f;
		else if (threshold < 0.0f) threshold = 0.0f;
		thresholdLevelInt = threshold * 32767.0f;
	}
	void attack(float milliseconds) {
		if(milliseconds <=  0.0f) {
			milliseconds = 0.01f; 
		}
		attackTimeDelta = 1.0f / (milliseconds / (1000.0f / (AUDIO_SAMPLE_RATE / AUDIO_BLOCK_SAMPLES)));
	}
	void hold(float milliseconds) {
		if(milliseconds <=  0.0f) {
			milliseconds = 0.01f; 
		}
		holdTimeDelta = 1.0f / (milliseconds / (1000.0f / (AUDIO_SAMPLE_RATE / AUDIO_BLOCK_SAMPLES)));
	}
	void release(float milliseconds) {
		if(milliseconds <=  0.0f) {
			milliseconds = 0.01f; 
		}
		releaseTimeDelta = 1.0f / (milliseconds / (1000.0f / (AUDIO_SAMPLE_RATE / AUDIO_BLOCK_SAMPLES)));
	}
	void floor(float gain) {
		if(gain < 0.0f)
			gain = 0.0f;
		if(gain > 1.0f)
			gain = 1.0f;
		floorGain = gain;
	}
	using AudioStream::release;
	virtual void update(void);
private:
	audio_block_t *inputQueueArray[1];
	int16_t thresholdLevelInt;
	float attackTimeDelta;
	float holdTimeDelta;
	float releaseTimeDelta;
	float floorGain;
	int32_t state;
	//Transient variables
	float currentGain;
	float currentHoldTime;
};

#endif