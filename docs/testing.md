# ElektroCaster testing guide

This guide lists manual and lightweight checks to run before and after refactors. Most functional tests require the physical ElektroCaster hardware.

## General pre-refactor checklist

1. Confirm the intended board(s): HID, CTL, Audio, or documentation only.
2. Record the current branch and `git status`.
3. Identify whether the change touches protected areas:
   - pin mappings,
   - serial protocols,
   - Audio Library routing or `AudioConnection`s,
   - fretboard scanning/debounce,
   - clock timing,
   - Kickup timing.
4. If protected behavior is touched, define an explicit hardware test plan before changing code.
5. Prefer one subsystem per pull request.

## Documentation-only checks

Can run without hardware:

- Verify changed file list contains only intended documentation files.
- Search for accidental firmware edits.
- Run `git diff --stat` and inspect `git diff`.
- Check Markdown readability manually.

## HID input test

Hardware required: HID board connected to CTL board or serial monitor instrumentation.

Purpose: verify controls still decode correctly.

Procedure:

1. Power the system in a safe state with actuators disabled if possible.
2. Move each analog control across its range.
3. Toggle each digital switch/button.
4. Rotate each encoder clockwise and counter-clockwise.
5. Rotate each 12-position rotary switch through all positions.
6. Observe decoded values on CTL debug output or display.

Expected results:

- Digital values change once per switch transition.
- Analog values change smoothly and remain in the expected `0..200` transmitted range.
- Encoder deltas have correct sign and no stuck values.
- Rotary switch states map to stable positions, likely `0..11`.

## Fretboard scan test

Hardware required: CTL board connected to the fretboard/string contact matrix.

Purpose: verify string/fret contact scanning, debouncing, and press/release behavior.

Procedure:

1. Enable a debug display or serial output for `strPrs`, `frtPrs`, and string index if available.
2. Press and release each string at several frets including low, middle, and high positions.
3. Test open-string release behavior.
4. Test fast repeated presses on the same string.
5. Test multiple simultaneous string presses.

Expected results:

- Pressed fret maps to the expected string and fret number.
- Release returns to zero state.
- No excessive double triggers.
- Behavior is unchanged before and after a refactor.

## LED fretboard test

Hardware required: CTL board and LED fretboard.

Purpose: verify LED mapping, colors, brightness, and frame update behavior.

Procedure:

1. Run a known LED mode, such as scale or grid display.
2. Verify all 150 LEDs can light.
3. Verify each string/fret coordinate maps to the expected physical LED.
4. Check brightness and color compensation visually.
5. Observe whether LED updates interfere with controls, clock, or audio.

Expected results:

- No missing or misaddressed LEDs.
- No unexpected color channel swaps.
- No visible stalls after documentation-only or behavior-neutral changes.

## Kickup test

Hardware required: CTL board, Kickup driver hardware, and safe test conditions.

Safety note: Kickup uses solenoids and can draw significant current. Use current limiting, dummy loads, or reduced power when appropriate.

Purpose: verify Kickup pulse timing and cue sequencing.

Procedure:

1. Disable or physically isolate strings if mechanical motion is unsafe.
2. Trigger each string individually.
3. Trigger multiple strings at once.
4. Use a logic analyzer or oscilloscope on Kickup pins if available.
5. Confirm pulse duration and inter-string cue spacing against documented values.

Expected results:

- Each selected string receives one pulse.
- Multiple queued kicks are staggered rather than fired simultaneously.
- No stuck HIGH outputs.

## Audio control test

Hardware required: CTL and Audio boards connected through their serial link.

Purpose: verify CTL-to-Audio command decoding.

Procedure:

1. Send or trigger known CTL commands for envelope trigger, string press, volume, filter, envelope parameter, string gain, bow mode/on, and LFO where applicable.
2. Observe Audio board debug output or audible behavior.
3. Confirm no command causes serial lockup.
4. If using debug output, keep prints minimal to avoid changing timing.

Expected results:

- Audio board receives correct command IDs and payloads.
- Values remain within expected scaled ranges.
- No blocking parser hang when commands are sent normally.

## Audio I/O test

Hardware required: Audio board, audio codec, hexaphonic input source or test signal, output monitoring.

Purpose: verify audio routing and per-string processing.

Procedure:

1. Feed a known signal into each string input channel one at a time.
2. Confirm each string path reaches the expected output/summed output.
3. Test envelope, filter, and gain changes.
4. Monitor processor and memory usage if debug instrumentation is enabled.

Expected results:

- Each input channel maps to the expected string.
- Output routing matches pre-refactor behavior.
- No unexpected clipping, silence, channel swap, or instability.

## MulEBow test

Hardware required: Audio board, coil/MulEBow actuator path, and safe actuator power setup.

Purpose: verify electromagnetic sustain actuator control.

Procedure:

1. Start with safe actuator levels.
2. Toggle Bow on/off from CTL controls.
3. Press strings and observe whether coil oscillator amplitude responds only for active strings.
4. Test bow mode changes if currently implemented in the UI.
5. Monitor for overheating or unintended continuous drive.

Expected results:

- Bow off results in no coil drive.
- Bow on drives only expected active strings.
- No stuck actuator output after release or mode changes.

## Tests possible without hardware

- StrHold arpeggiator regression test: `python3 tests/str_arp_test.py` (requires `g++` with AddressSanitizer and UndefinedBehaviorSanitizer). This compiles the actual CTL arpeggiator, MIDI output and fret-event handlers with hardware stubs. It checks independent held/live sequences, remembered pitches, order groups, repeats, mute, Hold release, zero-step behavior, sync, sensing-mode MIDI suppression and sequence bounds. In environments where LeakSanitizer cannot run under tracing, use `ASAN_OPTIONS=detect_leaks=0 python3 tests/str_arp_test.py`; address and undefined-behavior checks remain enabled.
- Documentation diff review.
- Markdown review.
- Static file inventory with `rg --files`.
- Git checks: `git status --short`, `git diff --stat`, `git diff --check`.
- Future host-side protocol encode/decode tests, if protocol code is extracted into testable modules.
- Future song serialization tests, if SD layout is abstracted into host-testable code.

## StrHold arpeggiator hardware test

Affected board: CTL (Teensy 4.1). The HID link supplies existing controls; the Audio link carries existing envelope commands. USB MIDI carries the notes. Pin mappings, serial formats, fretboard scanning/debounce, clock intervals and Kickup pulse/queue timing are unchanged.

1. Select an arpeggiator pattern, enable string automation and start the clock. Monitor USB MIDI and the existing CTL-to-Audio envelope commands; observe Kickup with actuator power initially disconnected.
2. Hold Shift and fret one string, release Shift first, then release the string. Confirm that MIDI continues at its last pressed pitch without envelope triggers or Kickup requests for that string.
3. Add a second held string. Confirm that both held strings form one sequence and keep their last pitches after release. Change the fret on one held string and confirm that its MIDI pitch updates.
4. Play additional strings without Shift. Confirm that their normal arpeggio runs alongside the held sequence. Use different `tmDv`/`repeat` values and equal positive `order` values across held/live strings; confirm independent progression and Note Off handling. Normal strings should still trigger their envelopes and enabled Kickup.
5. Test `simple`, `back`, `mirror`, `rev mirror`, positive `order` groups within the held sequence, `steps = 0`, mute/unmute and pattern/sync changes. Held strings must remain MIDI-only.
6. Activate Shift while the held strings are released to clear Hold. Confirm Note Off for the held notes and continued normal playback. Also stop the clock and confirm that no MIDI notes remain active.
7. Return to sequencer mode and confirm its existing StrHold behavior.
8. In arpeggiator mode, switch Auto off while the clock keeps running. Confirm that held strings continue their MIDI sequence and repeats without restarting or triggering envelopes/Kickup. Play a non-held string manually and confirm that the held sequence does not cut off its MIDI note, including when its arpeggiator track is muted. Clear Hold and check Note Off; re-enable Auto and confirm normal arpeggio playback resumes on the configured clock phase.

## Tests requiring hardware

- HID input test.
- Fretboard scan test.
- LED fretboard test.
- Kickup test.
- CTL-to-Audio and Audio-to-CTL live protocol test.
- Audio I/O test.
- MulEBow test.
