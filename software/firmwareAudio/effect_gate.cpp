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

#include "effect_gate.h"
#include "dspinst.h"

#if defined(KINETISK)
#define MULTI_UNITYGAIN 65536

static void applyGain(int16_t *data, int32_t mult)
{
	uint32_t *p = (uint32_t *)data;
	const uint32_t *end = (uint32_t *)(data + AUDIO_BLOCK_SAMPLES);

	do {
		uint32_t tmp32 = *p; // read 2 samples from *data
		int32_t val1 = signed_multiply_32x16b(mult, tmp32);
		int32_t val2 = signed_multiply_32x16t(mult, tmp32);
		val1 = signed_saturate_rshift(val1, 16, 0);
		val2 = signed_saturate_rshift(val2, 16, 0);
		*p++ = pack_16b_16b(val2, val1);
	} while (p < end);
}

#elif defined(KINETISL)
#define MULTI_UNITYGAIN 256

static void applyGain(int16_t *data, int32_t mult)
{
	const int16_t *end = data + AUDIO_BLOCK_SAMPLES;

	do {
		int32_t val = *data * mult;
		*data++ = signed_saturate_rshift(val, 16, 0);
	} while (data < end);
}

#endif

void AudioEffectGate::update(void)
{
	audio_block_t *block;
	uint32_t i;
	int16_t maxAbsSample = 0;
	bool processNextState = true;

	block = receiveWritable();
	if (!block) return;

	if(thresholdLevelInt > 0){		//Only process gate if threshold is non-zero
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
			if(abs(block->data[i]) > maxAbsSample)
				maxAbsSample = abs(block->data[i]);
		}
		
		while(processNextState){
			processNextState = false;
			switch (state) {
				case S_Floor:
					currentGain = floorGain;
					if(maxAbsSample > thresholdLevelInt) {
						state = S_Attack;
						processNextState = true;
					}					
					break;
				case S_Attack:
					currentGain += attackTimeDelta;
					if(currentGain >= 1.0f) {
						currentGain = 1.0f;
						state = S_Hold;
						processNextState = true;
					}
					break;
				case S_Hold:
					currentGain = 1.0f;
					if(maxAbsSample > thresholdLevelInt)
						break;			//Re-triggered. Also prevents infinite loop.
					currentHoldTime += holdTimeDelta;
					if(currentHoldTime >= 1.0f) {
						currentHoldTime = 0.0f;
						state = S_Release;
						processNextState = true;
					}
					break;
				case S_Release:
					if(maxAbsSample > thresholdLevelInt) {
						state = S_Attack;
						processNextState = true;
						break;			//Re-triggered
					}
					currentGain -= releaseTimeDelta;
					if(currentGain <= floorGain) {
						currentGain = floorGain;
						state = S_Floor;
						processNextState = true;
					}
					break;
			}
		}
		
		if(currentGain != 1.0f){
#if defined(KINETISK)
			applyGain(block->data, currentGain * 65536.0f);
#elif defined(KINETISL)
			applyGain(block->data, currentGain * 256.0f);
#endif
		}
	}
	
	transmit(block);
	release(block);
}



