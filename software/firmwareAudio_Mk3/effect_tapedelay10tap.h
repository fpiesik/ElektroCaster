/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
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
 * IMPLIED, INCLUDIu8g2.clearBuffer();NG BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Modified to extend to 10 taps PMF 16-03-2020
 * Modified for single samples delay line (rather than blocks) and tape delay like behaviour PMF 02-09-2020
 * added delayfade to fade between old and new delay time PMF 04-09-2020
 */

#ifndef effect_tapedelay10tap_h_
#define effect_tapedelay10tap_h_
#include "Arduino.h"
#include "AudioStream.h"

#define DELAY_NUM_TAPS 1  // max numer of taps / channels

class AudioEffectTapeDelay10tap : public AudioStream {
 public:
  AudioEffectTapeDelay10tap(void) : AudioStream(1, inputQueueArray) {}
  // initialise the delay line
  void begin(int16_t *delay_line, uint32_t max_delay_length);
  // activate a tap and/or change time with a fade between old and new time (no clicks), transition time in millis
  uint32_t delayfade(uint8_t channel, float milliseconds, float transition_time);
  // disable a tap
  void disable(uint8_t channel);
  // main update routine
  virtual void update(void);

 private:
  audio_block_t *inputQueueArray[1];
  uint32_t delay_samples[DELAY_NUM_TAPS];          // actual # of sample delay for each channel
  uint32_t desired_delay_samples[DELAY_NUM_TAPS];  // desired # of sample delay for each channel
  int16_t fade_length_samples[DELAY_NUM_TAPS];     // how long to fade in samples
  int16_t fade_count[DELAY_NUM_TAPS];              // count down for fades
  uint32_t max_delay_length_samples;               // lenght of the delay line in samples
  uint32_t write_index;                            // write head position
  uint16_t activemask;                             // which taps/channels are active
  int16_t *sample_delay_line;                      // pointer to delay line
};
#endif
