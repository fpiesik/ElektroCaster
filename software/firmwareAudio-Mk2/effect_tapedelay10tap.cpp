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
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *
 * Modified to extend to 10 taps PMF 16-03-2020
 * Modified for single samples delay line (rather than blocks) and tape delay like behaviour PMF 02-09-2020
 * added delayfade to fade between old and new delay time PMF 04-09-2020
 */
#include "effect_tapedelay10tap.h"

#include <Arduino.h>

void AudioEffectTapeDelay10tap::begin(int16_t *delay_line, uint32_t max_delay_length) {
  sample_delay_line = delay_line;
  max_delay_length_samples = max_delay_length - 1;
  write_index = 0;
}

// activate a tap and/or change time with a fade between old and new time (no clicks), transition time in millis
uint32_t AudioEffectTapeDelay10tap::delayfade(uint8_t channel, float milliseconds, float transition_time) {
  if (channel >= DELAY_NUM_TAPS) return 0;
  if (milliseconds < 0.0) {
    milliseconds = 0.0;
  }
  if (transition_time < 0.0) transition_time = 0.0;
  uint32_t delay_length_samples = (milliseconds * (AUDIO_SAMPLE_RATE_EXACT / 1000.0)) + 0.5;
  if (delay_length_samples > max_delay_length_samples) delay_length_samples = max_delay_length_samples;
  fade_length_samples[channel] = fade_count[channel] = (transition_time * (AUDIO_SAMPLE_RATE_EXACT / 1000.0)) + 0.5;
  // enable disabled channel
  if (!(activemask & (1 << channel))) {
    desired_delay_samples[channel] = delay_samples[channel] = delay_length_samples;
    fade_count[channel] = 0;
    activemask |= (1 << channel);
  } else {
    desired_delay_samples[channel] = delay_length_samples;
  }
  return delay_samples[channel];
}

void AudioEffectTapeDelay10tap::disable(uint8_t channel) {
  if (channel >= DELAY_NUM_TAPS) return;
  // disable this channel
  activemask &= ~(1 << channel);
};

void AudioEffectTapeDelay10tap::update(void) {
  audio_block_t *input, *output;
  int16_t *input_data_pointer, *output_data_pointer;
  uint32_t read_index, desired_read_index, start_index;
  uint8_t channel;
  float fade_multiplier;

  if (sample_delay_line == NULL) return;
  // grab a copy of the write_index starting poisition
  start_index = write_index;

  // write incoming block of samples to buffer
  input = receiveReadOnly();
  if (input) {
    input_data_pointer = input->data;
    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      sample_delay_line[write_index++] = *input_data_pointer++;
      if (write_index >= max_delay_length_samples) write_index = 0;
    }
    release(input);
  }

  // process each tap and write out delayed samples
  for (channel = 0; channel < DELAY_NUM_TAPS; channel++) {
    // check if channel is active
    if (!(activemask & (1 << channel))) continue;
    output = allocate();
    if (!output) continue;
    output_data_pointer = output->data;
    // position the read head for this channel / tap
    read_index = ((start_index - delay_samples[channel] + max_delay_length_samples) % max_delay_length_samples);
    // if fading between current delay to desired , position desired read head
    if (fade_count[channel] > 0) desired_read_index = ((start_index - desired_delay_samples[channel] + max_delay_length_samples) % max_delay_length_samples);
    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      // if fading, cross mix in fade_length_samples steps
      if (fade_count[channel] > 0) {
        // calculate a multipier for the fade
        fade_multiplier = (float)fade_count[channel] / fade_length_samples[channel];
        // read the two points from the delay line, crossfade and send to the output block
        *output_data_pointer++ =
            (int16_t)(sample_delay_line[read_index] * fade_multiplier) + (int16_t)(sample_delay_line[desired_read_index] * (1 - fade_multiplier));
        fade_count[channel]--;
        if (fade_count[channel] == 0) {
          delay_samples[channel] = desired_delay_samples[channel];
          read_index = desired_read_index;
        }
        // increment and wrap around the desired_read_index
        desired_read_index++;
        if (desired_read_index >= max_delay_length_samples) desired_read_index = 0;
      } else {
        // read delay line and send sample to the output block
        *output_data_pointer++ = sample_delay_line[read_index];
      }
      // increment and wrap around the main read index
      read_index++;
      if (read_index >= max_delay_length_samples) read_index = 0;
    }
    transmit(output, channel);
    release(output);
  }
}
