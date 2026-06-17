# ElektroCaster architecture

This document summarizes the firmware architecture as currently inferable from the repository. It is documentation only and does not define new behavior.

## System overview

ElektroCaster is a hardware/firmware electric guitar platform with:

- a 150-pixel LED fretboard,
- touch-sensitive fretboard detection through string/fret contacts,
- Kickup solenoid string actuators,
- a MulEBow electromagnetic sustain actuator,
- hexaphonic pickup/audio processing,
- many body controls,
- an OLED display.

The firmware is split across three microcontrollers.

## Microcontroller roles

### `software/firmwareHid`

Likely target: Teensy 3.5.

Responsibilities:

- Read body controls:
  - 22 analog inputs,
  - 19 digital inputs,
  - 8 encoders are instantiated in code, although `nEnc` is currently `7`, which should be treated as an open inconsistency until verified on hardware,
  - 3 analog resistor-ladder rotary switch values.
- Send changed control values to `firmwareCtl` over a serial link.
- Own the OLED display hardware through U8g2.
- Receive display drawing commands from `firmwareCtl` using AsciiMassage packets and render them locally.

Important files:

- `software/firmwareHid/firmwareHid.ino`: pin arrays, encoder instances, input scanning loop, display initialization.
- `software/firmwareHid/send.ino`: HID-to-CTL byte protocol.
- `software/firmwareHid/receive.ino`: CTL-to-HID display command receiver.
- `software/firmwareHid/display.ino`: local U8g2 drawing helpers.

### `software/firmwareCtl`

Likely target: Teensy 4.1.

Responsibilities:

- Main system state.
- LED fretboard control through FastLED/WS2812Serial.
- Fretboard contact scanning and debouncing.
- Kickup cueing and solenoid timing.
- MulEBow control messages sent to the audio board.
- Sequencer, scale, string setup, song, and pattern state.
- SD card song loading/saving.
- USB MIDI and hardware MIDI handling.
- Serial coordination with HID and Audio boards.

Important files:

- `software/firmwareCtl/firmwareCtl.ino`: large global state block, setup, main loop, hardware initialization.
- `software/firmwareCtl/04_hid.ino`: fretboard scan/debounce plus HID processing and musical actions.
- `software/firmwareCtl/2audio.ino`: CTL-to-Audio command senders.
- `software/firmwareCtl/13_serialEvents.ino`: serial receivers from Audio and HID.
- `software/firmwareCtl/01_actuators.ino`: Kickup cueing and pulse handling.
- `software/firmwareCtl/02_clock.ino`: internal clock, MIDI clock, sync points, pattern scheduling.
- `software/firmwareCtl/03_display.ino`: display update and drawing command senders.
- `software/firmwareCtl/05_ledfrets.ino`: LED fretboard update logic.
- `software/firmwareCtl/14_song.ino`: binary SD song save/load format.

### `software/firmwareAudio`

Likely target: Teensy 4.1.

Responsibilities:

- Hexaphonic audio input and processing using the Teensy Audio Library.
- TDM audio input and output through `AudioInputTDM`, `AudioOutputTDM`, and `AudioControlCS42448`.
- Per-string input gain, envelope, filter, ring/amplitude modulation, pitch analysis, RMS analysis.
- MulEBow/coil oscillator outputs.
- Receive control commands from `firmwareCtl`.
- Send pitch and amplitude analysis data back to `firmwareCtl`.

Important files:

- `software/firmwareAudio/firmwareAudio.ino`: global audio objects, state, static output summing connections.
- `software/firmwareAudio/0Setup.ino`: audio hardware setup and per-string `AudioConnection` construction.
- `software/firmwareAudio/yLoop.ino`: serial command parser, analysis loop, MulEBow state handling.
- `software/firmwareAudio/2ctl.ino`: Audio-to-CTL status senders.
- `software/firmwareAudio/envelopes.ino`, `filter.ino`, `fx.ino`, `lfos.ino`: DSP parameter helpers.

## Data flow

```text
Body controls + OLED hardware
        |
        | firmwareHid Serial6 @ 115200 baud
        v
firmwareCtl Serial7 @ 115200 baud
        |
        | firmwareCtl Serial1 @ 250000 baud
        v
firmwareAudio Serial1 @ 250000 baud
```

Additional flows:

- `firmwareCtl` sends display drawing commands to `firmwareHid` using AsciiMassage packets.
- `firmwareHid` sends HID control changes to `firmwareCtl` using compact byte messages.
- `firmwareCtl` sends audio control, string state, envelope, filter, volume, and bow commands to `firmwareAudio` using compact byte messages.
- `firmwareAudio` sends pitch and amplitude estimates back to `firmwareCtl`.
- `firmwareCtl` also handles USB MIDI and hardware MIDI.

## Known technical risks

- Many global variables in `firmwareCtl` and `firmwareAudio` make dependencies implicit.
- Several files mix hardware access, state updates, UI handling, musical logic, and protocol handling.
- Pin assignments and hardware mappings are embedded directly in firmware files.
- Serial protocols use magic byte values and do not appear to have checksums, versioning, or documented recovery behavior.
- Several serial parsers use blocking `while(Serial.available() == 0)` waits, which can hang if bytes are lost or boards reset mid-message.
- `firmwareCtl/04_hid.ino` couples fretboard scanning, debounce, MIDI, Kickup, audio triggering, and sequencer editing.
- `firmwareCtl/02_clock.ino` calls display updates from the internal clock path; timing impacts should be measured before changing it.
- `firmwareAudio/0Setup.ino` builds many audio connections dynamically; audio routing changes are high risk.
- `firmwareCtl/14_song.ino` stores song data as an implicit binary layout without an evident version header.

## Refactor safety guidance

- Documentation-only changes should not touch firmware source files.
- For firmware refactors, start by naming constants and documenting existing behavior before changing control flow.
- Treat pin mappings, serial payloads, audio routing, Fretboard scan timing, Clock timing, and Kickup pulse timing as protected behavior.
