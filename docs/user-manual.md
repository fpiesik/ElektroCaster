# ElektroCaster – User Manual

> **Status and scope:** This manual describes only behavior that can be established from the current firmware source code. The controls are only partially labeled in the code, so their firmware indices are also used here. Statements marked **“unclear”**, **“currently has no effect”** or **“to be verified”** deliberately avoid making assumptions about the hardware.

## Table of contents

1. [Safety and important limitations](#1-safety-and-important-limitations)
2. [System overview](#2-system-overview)
3. [Controls and terminology](#3-controls-and-terminology)
4. [Global switches and buttons](#4-global-switches-and-buttons)
5. [Faders, pots and joystick](#5-faders-pots-and-joystick)
6. [String Setup and tuning aid](#6-string-setup-and-tuning-aid)
7. [Scale and play view](#7-scale-and-play-view)
8. [Generic sequencer](#8-generic-sequencer)
   - [Structure](#81-structure)
   - [Pattern rotary switches](#82-pattern-rotary-switches)
   - [Track parameters via string encoders](#83-track-parameters-via-string-encoders)
   - [Setting and editing steps](#84-setting-and-editing-steps)
   - [Step parameters (`str`)](#85-step-parameters-str)
   - [String button functions (`btn`)](#86-string-button-functions-btn)
   - [Copying patterns](#87-copying-patterns)
9. [String arpeggiator](#9-string-arpeggiator)
   - [Enabling the arpeggiator](#91-enabling-the-arpeggiator)
   - [Global arpeggiator functions](#92-global-arpeggiator-functions)
10. [Playing, fretboard sensing and actuators](#10-playing-fretboard-sensing-and-actuators)
11. [Clock and synchronization](#11-clock-and-synchronization)
12. [MIDI functions](#12-midi-functions)
13. [Songs and SD card](#13-songs-and-sd-card)
14. [Reading the OLED and LED displays](#14-reading-the-oled-and-led-displays)
15. [Known current limitations](#15-known-current-limitations)
16. [Recommended initial function test](#16-recommended-initial-function-test)

## 1. Safety and important limitations

- The ElektroCaster is a prototype with six **Kickup solenoids** and six electromagnetic **MulEBow coils**. Operate the actuators only with a suitably rated power supply and driver hardware. Disconnect actuator power before working on strings, the bridge, coils or wiring.
- Kickup can strike a string automatically. Keep hands and loose objects clear of moving parts.
- MulEBow generates a continuous electromagnetic signal. Switch it off immediately if unusual heating or noise occurs.
- No OLED error or protection messages for SD, audio or communication faults can be identified in the firmware code.
- The actual layout, labels, switch polarity and string numbering are not fully documented. Check all control assignments at low volume before stage use or operation at high volume.
- Internally, the firmware uses **string indices 0 to 5**. OLED values are usually displayed in reverse spatial order. This manual therefore uses the neutral terms “string 0 … 5” rather than high/low E string.

## 2. System overview

The ElektroCaster can be played like a regular guitar. It also processes each string separately and provides:

- a touch-sensitive fretboard with 21 sensed fret positions,
- 150 RGB LEDs, corresponding to 25 LED positions per string,
- hexaphonic filter, envelope and ring modulation processing,
- Kickup for mechanically striking individual strings,
- MulEBow for electromagnetically exciting fretted strings,
- USB-MIDI output and USB-MIDI input,
- three parallel generic sequencer instances with six patterns each,
- an alternative string arpeggiator with six patterns,
- 100 song slots on an SD card,
- scale, MIDI and sequencer displays on the LED fretboard.

At startup, the main controller loads **song 0**, followed by the separately saved string tuning and string levels. The internal default is 90 BPM; the clock starts only when the play button is pressed or a corresponding control sequence is executed.

## 3. Controls and terminology

### 3.1 Overview

| Control | Count | Name used in this manual |
| --- | ---: | --- |
| Slider | 10 | Fader 1 … 10 |
| Potentiometer | 7 | Pot 1 … 7 |
| Joystick | 1, two axes | Joystick X/Y |
| Rotary encoder | 8 | Display encoder, six string encoders, master encoder |
| 12-position rotary switch | 3 | Pattern switch 1 … 3 |
| Digital inputs | 19 | Display button, Shift, three actuator/Auto switches, three transport buttons, pot switch, memory button, pot encoder button, two fretboard switches, six string encoder buttons |

The “fader/pot” assignments follow the comments in the controller code. One analog input is listed there as “Pot 7”; the other six are labeled Pot 1, 2, 3, 5 and 6, plus another LFO control identifiable only by its function. A physical label for “Pot 4” cannot be reliably established from the code.

### 3.2 Encoders

- **Display encoder:** The single encoder next to the OLED. Depending on the view, turning it changes a value or the selected function row. Pressing it switches between these two levels.
- **String encoders 0 … 5:** One encoder per string. They change the currently selected string parameter. Their values appear on the OLED from left to right in the order string 5 … 0.
- **Master encoder:** Changes the current string parameter for all strings together. In instance 1's string sequencer and in the arpeggiator, strings with StringHold enabled are skipped. In step-edit mode, it can instead edit the selected step parameter.
- The HID firmware divides encoder movements by four. A perceptible detent movement therefore results in a change only after four raw steps.

### 3.3 Using the display encoder

The following generally applies in the scale, sequencer and arpeggiator views:

1. **Display button released:** Turning changes the value in the highlighted row.
2. **Display button pressed:** Turning moves the selection highlight between rows.

In String Setup, turning the display encoder switches directly between tuning, string level and BPM; the button state is not evaluated there.

## 4. Global switches and buttons

### 4.1 Shift button

The button below the display encoder sets the global `shift` state. Shift modifies the following functions:

- together with the **MulEBow switch**: select internal/external clock,
- together with the **Kickup switch**: select full/simplified fretboard sensing,
- when editing a sequencer step: delete the step or use the display encoder to change all steps of the selected string(s),
- when changing songs: save song 0 before switching, instead of the currently selected slot,
- together with the memory button and the pressed display button: restore the default song in RAM only.

The code evaluates a state, not an explicit hold duration. Whether the physical control is a momentary button or a latching switch must be checked on the instrument.

### 4.2 Three-switch group

| Switch | Without Shift | With Shift |
| --- | --- | --- |
| Left | Automation for instance 1: turn the generic string sequencer or, when selected, the arpeggiator on/off | No additional special function |
| Center | MulEBow on/off | Also sets `extClk`: off = internal clock, on = external USB-MIDI clock |
| Right | Kickup on/off | Also sets the fretboard sensor mode: off = detect fret position, on = detect pressed/released only |

With the clock running, StringHold strings continue to output MIDI in the selected string sequencer or arpeggiator even when automation is off.

**Important:** With Shift active, MulEBow or Kickup is still switched as well. Clock or sensor mode selection is not a separate secondary function that leaves the actuator state unchanged.

### 4.3 Three-button group

| Button | Function |
| --- | --- |
| Right | Schedules synchronization of all three sequencer instances and the arpeggiator at the next bar boundary |
| Center | Toggles start/stop; start sends USB-MIDI Start, followed by 24 clock ticks per quarter note; stop sends USB-MIDI Stop and All Notes Off |
| Left | Cycles through sequencer instances 1, 2, 3 and switches to their operating mode |

### 4.4 Fretboard switches

- **Edit/Play switch (`fbrdMode`):**
  - `0` = Play. Touched frets generate envelope, MIDI and Kickup events depending on the automation state; the LED fretboard shows the scale/note view.
  - `1` = Step Edit. The first 16 fret positions are used to set and edit sequencer steps; the LED fretboard shows the grid. The main audio output is muted in this mode, except during alternative arpeggiator operation.
- **Hold sequencer view (`fbrdSeqVHld`):** Shows the sequencer or arpeggiator grid even in play mode. The code comment explicitly describes this view as **not editable**.

### 4.5 Pot switch and encoder buttons

The **pot switch** selects the function of the six string encoder buttons:

- Position `1`: A string encoder button directly selects an encoder function. Because of the reversed mapping, the six buttons select function indices 5 … 0. The generic sequencer has only five valid encoder functions; the additional index 5 is set but has no implemented effect. The same applies to the additional index 5 in the arpeggiator.
- Position `0`: A string encoder button performs the string function currently selected under `btn` on the OLED.

The separate **pot encoder button** performs the currently selected generic string button function for all six strings at once. Even during arpeggiator operation, it calls the generic sequencer function, not the arpeggiator function.

## 5. Faders, pots and joystick

All analog values are internally normalized to approximately 0 … 1.

| Control | Function | Output range / note |
| --- | --- | --- |
| Fader 1 | USB-MIDI CC 3, channel 1 | 0 … 127 |
| Fader 2 | USB-MIDI CC 4, channel 1 | 0 … 127 |
| Fader 3 | USB-MIDI CC 5, channel 1 | 0 … 127 |
| Fader 4 | USB-MIDI CC 6, channel 1 | 0 … 127 |
| Fader 5 | Filter envelope decay | Scaled up to approx. 5000 ms |
| Fader 6 | Amplitude envelope decay | Scaled up to approx. 5000 ms |
| Fader 7 | Filter envelope attack | Scaled up to approx. 1000 ms |
| Fader 8 | Amplitude envelope attack | Scaled up to approx. 1000 ms |
| Fader 9 | Filter envelope amount | Crossfade between constant and envelope control signal |
| Fader 10 | Amplitude envelope amount | Crossfade between dry ring modulation signal and envelope signal |
| Joystick X | USB-MIDI CC 9, channel 1 | 0 … 127 |
| Joystick Y | USB-MIDI CC 10 **and** Channel Aftertouch, channel 1 | Both 0 … 127 |
| Pot 1 | Overall audio volume | Nonlinear; the controller limits the maximum transmitted value to avoid clipping |
| Pot 2 | Filter resonance | Audio scaling approx. 0 … 7 |
| Pot 3 | Filter octave control | Audio scaling approx. 0 … 7 |
| Pot 4 (not explicitly labeled in code comments) | LFO pitch offset | Approx. −12 … +12 semitones |
| Pot 5 | USB-MIDI CC 7, channel 1 | 0 … 127 |
| Pot 6 | LFO/ring modulation depth | Amplitude increases as offset decreases accordingly |
| Pot 7 | LFO octave range | Internally approx. −5 … +4 octaves |

When a string is fretted manually, the firmware holds its amplitude and filter envelopes at sustain, even if the decay control is set shorter. On release or automatic triggering, the normal decay/sustain logic applies again.

## 6. String Setup and tuning aid

### 6.1 Opening String Setup

String Setup is operating mode 0. It is opened via **pattern switch 2 or 3, position 6** (`val == 6`). Pattern switch 1 instead switches to arpeggiator operation from position 6 onward.

### 6.2 Parameters

The display encoder selects one of three highlighted OLED rows:

1. **Tuning:** String encoders shift each string's target tuning in MIDI semitones. The OLED shows only the note name, without the octave.
2. **String level:** Range 0 … 50; the value is transmitted to the audio controller as a gain of 0.00 … 1.00.
3. **BPM:** Range 1 … 250; every string encoder changes the same global tempo value.

In String Setup, the master encoder always transposes all six tunings together, regardless of the currently highlighted OLED row.

On leaving String Setup, tuning and string levels are saved separately. At system startup, they are loaded after song 0 and therefore override the corresponding values stored in the song.

### 6.3 LED tuning aid

In String Setup:

- LED position 0 marks the configured open string in its note color,
- position 12 serves as a center reference,
- the detected pitch is displayed in color when the input level is sufficient,
- an adjacent LED indicates whether the detected pitch is above or below the target range.

The direction of “sharp/flat” relative to the physical orientation of the neck cannot be reliably determined from the code alone. Pitch detection accepts only measurements with high analysis confidence; the display may freeze if the string is quiet or unstable.

## 7. Scale and play view

The scale view appears in sequencer modes when the fretboard switch is set to Play and “Hold sequencer view” is off.

### 7.1 Adjustable fields

With the display button pressed, select one of six fields; with the button released, change its value:

| OLED field | Function | Values |
| --- | --- | --- |
| Root | Tonal center | C, C#, D, D#, E, F, F#, G, G#, A, A#, B |
| Scale | Scale type | off, root, pentatonic, major, minor, hrm minor, mel minor, altered, whole, wholeHalf |
| Degree | Mode/rotation within the scale | Depends on the scale |
| `clr` | Scale color | 0 = white, 1 = pitch-class colors |
| `sng` | Song slot | 0 … 99 |
| `src` | Source of additionally highlighted notes | 0 = received MIDI/string notes; 1 … 3 = active notes of sequencer instance 1 … 3 |

**Scale `off`:** Its data structure contains zero degrees. The degree calculation nevertheless uses modulo with this count. This combination is therefore prone to errors and should not be operated via the degree function until corrected.

### 7.2 Changing songs via the OLED

When turning the song field, the following happens before each change:

1. Without Shift: the current song slot is saved.
2. With Shift: slot 0 is saved.
3. The newly selected slot is then loaded.

Changing the song field is therefore **not a read-only operation** and can overwrite data. Back up the SD card before experimenting.

### 7.3 LED display

- The scale is drawn across all visible frets.
- Pitch classes can appear in color or monochrome.
- `src 0` overlays active USB-MIDI notes:
  - channels 1 … 6 are each assigned to a string,
  - channel 15 highlights a pitch class at all matching frets,
  - channel 16 highlights the exact MIDI note.
- `src 1 … 3` overlays the pitch classes currently output by the corresponding sequencer instance.
- The last three LED positions also provide a pattern display for the three instances.

## 8. Generic sequencer

### 8.1 Structure

There are three instances. Each has:

- six patterns,
- six string tracks,
- a maximum of 16 steps per track,
- independent length, time division, start offset, sync/pattern control and MIDI channel per track,
- per step: MIDI note/scale degree, repeat count, velocity and three CC values.

The left transport button selects instance 1 … 3. Pattern rotary switches 1 … 3 belong to the corresponding instances.

### 8.2 Pattern rotary switches

- Positions 0 … 5 select pattern 1 … 6 of the corresponding instance.
- With the clock running, the change is scheduled for the next quarter note; with the clock stopped, it takes effect immediately.
- With Shift, the selected pattern is set only as the edit pattern, not scheduled as the playback pattern.
- Positions 9 … 11 can synchronize the instance with another instance. The exact physical direction of rotation and useful cases for self-synchronization must be checked on the instrument.
- On pattern switches 2 and 3, position 6 opens String Setup.

Pattern switch 1 has an additional special role: positions 6 … 11 select the arpeggiator and its six patterns; the change is quantized like a pattern change.

### 8.3 Track parameters via string encoders

In step-edit/grid mode, `enc` on the OLED selects one of the following functions:

| Display | Meaning | Range |
| --- | --- | ---: |
| `len` | Track loop length | 1 … 16 steps |
| `tmDv` | Tick interval per step | `4`, `2`, `1`, `.75`, `/2`, `/3`, `/4`, `/6`, `/8`, `/12`, `/16`, `/24`, `/32`, `/48` |
| `offSt` | Start offset in the 16-step grid | 0 … 16 |
| `sync` | Pattern number that a channel-0 control track can request | 0 … 6; 0 = off, 1 … 6 = pattern 1 … 6 |
| `chn` | USB-MIDI channel | 0 … 16; channel 0 is internal control instead of MIDI output |

The time division labels are taken directly from the code. Internally, they refer to a 24-PPQN clock; for example, `1` corresponds to 96 ticks, `/4` to 24 ticks and `/16` to 6 ticks.

### 8.4 Setting and editing steps

1. Set the fretboard to **Edit**.
2. Press fret 1 … 16 on a string. This toggles the corresponding step.
3. While holding the fret:
   - turn the display encoder to change the parameter selected under `str`, or
   - use the string encoders. In step mode, the six encoders are assigned to the six step parameters.
4. Shift + fret contact deletes the step.
5. Shift + display encoder changes the selected parameter for all 16 steps of the string(s) currently held.

If multiple fret contacts are detected simultaneously on a string, changes made with the string/master encoders are applied to all steps of the selected string(s). This multiple-contact logic depends on the hardware.

### 8.5 Step parameters (`str`)

| Display | Meaning | Range / output |
| --- | --- | --- |
| `pStp` | MIDI note or continuous scale degree | 0 … 127 |
| `rpt` | Triggers per step | 1 … 6; step length remains unchanged |
| `vel` | Velocity | 0 … 50, scaled to 0 … 127 for MIDI |
| `c10` | Internal name; transmits CC 2 | 0 … 99 |
| `c11` | Internal name; transmits CC 3 | 0 … 99 |
| `c12` | Internal name; transmits CC 4 | 0 … 99 |

`pStp` now combines pitch and octave; the former separate `oct` parameter has been replaced by `rpt`. With `sclQ = 0`, `pStp` directly corresponds to the output MIDI note number. With `sclQ = 1`, the value is interpreted as a continuous degree of the selected, rotated scale: values beyond the first pass through the scale extend into correspondingly higher octaves. The OLED shows the resulting note with its octave.

`rpt = 1` triggers the step once at its start. Higher values distribute up to six triggers evenly within the same step duration; they do not extend the step. The additional triggers repeat the note, audio envelope and, where enabled, Kickup, but not the step's three CC outputs.

The OLED names `c10`, `c11`, `c12` do not match the controller numbers actually transmitted: 2, 3 and 4.

If the track channel is `0`, the three CC fields are not transmitted but instead control the following internally:

- first CC field: root (`value − 1`),
- second CC field: scale (`value − 1`),
- third CC field: scale degree (`value − 1`).

A CC/control value of 0 performs no action. On channel 0 with `sync` active, an enabled step can also request a pattern change.

### 8.6 String button functions (`btn`)

| Display | Effect when pressed |
| --- | --- |
| `mute` | Toggle track muted/active |
| `rnd` | Intended to randomize a track; in the current code, however, the target count is always 0, so the track's active steps are cleared |
| `sclQ` | Toggle scale quantization for the track |

With `sclQ = 0`, `pStp` is an absolute MIDI note. With `sclQ = 1`, it is a continuous degree of the currently selected, rotated scale.

### 8.7 Copying patterns

When the display encoder is pressed, the firmware remembers the currently playing patterns. If a different edit pattern is then selected and the display encoder is released, it copies the previously playing pattern into the selected edit pattern. This affects all three instances whose edit pattern differs from the previously active pattern. There is no confirmation prompt.

## 9. String arpeggiator

### 9.1 Enabling the arpeggiator

1. Turn pattern switch 1 to position 6 … 11 to select one of the six arpeggiator patterns.
2. Turn on the left switch in the three-switch group to enable automation.
3. In play mode, fret strings at the desired positions.

The arpeggiator builds its sequence dynamically from the strings currently pressed. Each triggered string step sends MIDI, triggers the audio envelope and, if enabled, Kickup.

Strings with **StrHold** enabled form their own arpeggio running in parallel. Each uses the last fret pressed in play mode and sends MIDI only; the envelope and Kickup are not triggered. The other pressed strings form the normal arpeggio sequence. Both sequences have their own cursor and use the configured values for `steps`, `tmDv`, `repeat`, `order`, `chn` and `mute`. Equal positive `order` values group strings only within the same sequence. For StrHold, `steps = 0` also suppresses manual triggering.

The StrHold arpeggio continues with the clock running even when Auto mode is off. The normal arpeggio sequence requires Auto mode; when Auto mode is off, the remaining strings can be played manually. Mute and clearing StringHold still apply to the held strings. Start/Stop controls the clock for both sequences.

### 9.2 Global arpeggiator functions

Under `str`:

| Display | Order |
| --- | --- |
| `simple` | Configured/pressed order forward |
| `back` | Reverse order |
| `mirror` | Forward, then backward, including the endpoints |
| `rev mirror` | Backward, then forward, including the endpoints |

Under `btn`:

- `mute`: toggle the string track,
- `randomise`: **currently has no effect** in the code.

Under `enc`:

| Display | Function | Range |
| --- | --- | ---: |
| `steps` | Number of steps for the string in the arpeggio sequence | 0 … 64 |
| `tmDv` | Duration for this string | `1`, `1.5`, `2`, `3`, `4`, `6`, `8`, `12`, `16`, `24`, `32`, `48` |
| `repeat` | Number of triggers per step; step length remains unchanged | 1 … 6 |
| `order` | Fixed order/group | 0 … 6 |
| `chn` | USB-MIDI channel | 0 … 16 |

`order = 0` orders strings by the sequence in which they are fretted. Equal positive `order` values group strings so they trigger together. `steps = 0` removes a string from the clocked sequence. Such a string can still trigger manually while the arpeggiator is active.

`repeat = 1` triggers each arpeggiator step once. Higher values distribute the triggers evenly within the duration selected via `tmDv`, without extending the step. For grouped strings, the additional triggers also apply to all strings in the same positive `order` group. The former `ser o par` parameter has been removed; parallel playback is configured exclusively through these `order` groups.

## 10. Playing, fretboard sensing and actuators

### 10.1 Full fret detection

In sensor mode `0`, the firmware detects the highest fret contacted simultaneously on each string. In play mode, a new contact generates:

- an audio envelope, unless sequencer/arpeggiator automation suppresses manual triggering,
- a USB-MIDI note with velocity 127,
- an optional Kickup pulse,
- a message to the audio controller containing pitch and fretting state.

The manual USB-MIDI note is `string tuning + fret number + 12`. When changing frets or releasing the string, its previous note is ended.

### 10.2 Simplified detection

In sensor mode `1`, the firmware detects only whether a string touches any fret. Since no reliable fret number is available, normal MIDI note events for fret positions are suppressed. This mode is accessible via Shift + Kickup switch.

### 10.3 Holding strings

If a string is fretted in play mode while Shift is active, the firmware sets `hold` for that string. While Shift remains active, string contact updates this state: fretting sets Hold, releasing clears Hold. If Shift is released first, Hold remains active even after the string is released. To clear it, activate Shift with the string released. In the generic sequencer, a held string uses its last known fret position as its MIDI note. In the arpeggiator, held strings form a separate MIDI arpeggio sequence without envelope or Kickup triggers.

### 10.4 Kickup

- The right switch in the three-switch group enables Kickup.
- Manual fretting events, the string sequencer, the arpeggiator and incoming USB-MIDI notes on channels 1 … 6 can trigger Kickup.
- MIDI channel 1 internally triggers string 5, channel 2 string 4, …, channel 6 string 0; the note number is not evaluated.
- Simultaneously requested strikes are processed one after another to reduce current peaks.

### 10.5 MulEBow

- The center switch in the three-switch group turns MulEBow on/off.
- With MulEBow on, the audio controller generates a coil signal at the reported pitch only for strings reported as fretted.
- On release, the string's signal is switched off.
- The global `bowMode` exists in the code, but no current control sends a change. No user-selectable MulEBow mode beyond on/off can therefore be documented.

## 11. Clock and synchronization

### 11.1 Internal clock

- Tempo: 1 … 250 BPM, default 90 BPM.
- Resolution: 24 PPQN.
- Start/Stop: center transport button.
- Start, Clock and Stop are output as USB-MIDI realtime messages.

### 11.2 External clock

1. Activate Shift.
2. Turn on the MulEBow switch to set `extClk = 1`.
3. Enable the clock with the center transport button.
4. Supply USB-MIDI Clock.

USB-MIDI Start resets the internal counter; USB-MIDI Stop resets it, clears received note displays and sends All Notes Off. However, the external Start/Stop handler does not set `clckOn` itself. Local transport enabling is therefore still required.

## 12. MIDI functions

### 12.1 Outgoing via USB-MIDI

- manual fretboard notes on the channel selected per track,
- sequencer notes from instances 2 and 3,
- instance 1 as string sequencer/arpeggiator control,
- sequencer CC 2, 3 and 4,
- fader CC 3 … 6 on channel 1,
- joystick CC 9/10 and Aftertouch on channel 1,
- pot CC 7 on channel 1,
- Program Change when loading songs 1 … 99: program number = song slot − 1,
- internal Start/Clock/Stop messages.

The hardware MIDI port is initialized but not read in the main loop, and the send functions use USB-MIDI. A current operating function for DIN/hardware MIDI therefore cannot be confirmed.

### 12.2 Incoming via USB-MIDI

- Note On/Off is stored for the LED display.
- Note On on channel 1 … 6 also triggers the audio envelope and Kickup for the assigned string. The note's pitch does not matter.
- Clock, Start and Stop control timing when external clock mode is enabled.
- CC 10 selects the root modulo 12.
- CC 11 selects the scale (`value modulo 16`, if valid) and scale degree (`value / 16`).
- CC 12 selects the scale degree modulo the number of scale degrees.
- CC 13 controls the LFO pitch offset.

Here, too, CC 11/12 combined with scale `off` can cause invalid calculations because the number of degrees is 0.

## 13. Songs and SD card

### 13.1 Stored data

Under the current file format, a song contains:

- root, scale, scale degree, color mode, BPM and LED source,
- all three sequencer instances with six patterns, track parameters, steps, step values and scale quantization,
- tuning and string levels,
- all six arpeggiator patterns, including global function selections and string parameters.

Files are named `SONG00.ECS` through `SONG99.ECS`. When writing, a temporary file is first created, read back and checked by CRC before it replaces the actual slot.

The current song format is **version 4**. The new sequencer and arpeggiator repeat parameters make files from older format versions incompatible; when loading such slots, the firmware falls back to the default song. Back up existing song files before changing the firmware and save the patterns again in the current format.

### 13.2 Memory button

The recessed button near the pots responds when pressed:

| Display button | Shift | Action |
| --- | --- | --- |
| Released | Off | Load the selected song slot |
| Released | On | Save the current state to **slot 0** |
| Pressed | Off | Load **slot 0** |
| Pressed | On | Restore the default song in RAM, without saving yet |

Saving the currently selected slot instead happens automatically when the song field on the OLED is turned to another value **without Shift**.

### 13.3 Errors and the default song

If a song file is missing, damaged, has an incorrect version/size/checksum or contains invalid values, the firmware loads the default song instead. The error status is visible only through the debug interface, not on the OLED.

Among other settings, the default song uses 90 BPM, C major, a colored scale, sequencer length 16, time division `/16`, empty steps, `pStp = 36`, `rpt = 1` and unmuted arpeggiator tracks with `steps = 1` and `repeat = 1`.

## 14. Reading the OLED and LED displays

- The OLED shows only the parameters relevant to the current context; there is no permanent status row for the clock, Kickup, MulEBow or external clock.
- A **filled** rounded frame marks the currently editable row in the sequencer/arpeggiator; an **empty** frame marks the row while the display button is pressed to select a function.
- In Step Edit, the right half of the OLED shows the note name/octave, repeat count, velocity, three CC values and `sclQ` for the last selected step.
- In the sequencer grid, faint colors mark the grid, note colors mark enabled steps, gray marks channel 0, subdued colors mark muted tracks and a bright overlay marks the current cursor position.
- The LED display fades out gradually rather than switching each pixel off abruptly.

## 15. Known current limitations

The following points are directly apparent from the active code and should be considered during operation:

1. String arpeggiator mode 1 as a separate `opMode` currently draws neither the OLED nor the LEDs; in practice, the arpeggiator is operated via pattern switch 1 within sequencer instance 1.
2. Arpeggiator `randomise` is not implemented.
3. In the current implementation, generic `rnd` clears the track instead of generating a random pattern.
4. Scale `off` cannot be reliably combined with step/degree operations.
5. The three pattern rotary switches share value ranges with special functions (arpeggiator, instance sync, String Setup). Mark the physical detent positions before live use.
6. Song changes and pattern copying can overwrite data without confirmation.
7. The firmware initializes the SD card before further main controller initialization and exits `setup()` early on failure. Normal operation therefore cannot be expected without a working SD card.
8. The physical string order, connector pinout and control labels must be verified on the actual build.

## 16. Recommended initial function test

1. Leave actuator power off initially; set the audio level low.
2. Insert the SD card and turn on the instrument. Check whether the OLED and LED fretboard respond.
3. Open String Setup via pattern switch 2 or 3, position 6; check string tuning and levels.
4. Switch to the scale/play view and check the root, scale and LED source.
5. Fret each string at several positions and observe USB-MIDI and LED responses.
6. Start the internal clock, select an empty test pattern and program individual steps.
7. Only then test Kickup with a current-limited actuator power supply.
8. Test MulEBow last, using short on-times and monitoring coil temperature.
9. Back up the SD card before changing songs and observe the unusual save/load combinations in section 13.2.
