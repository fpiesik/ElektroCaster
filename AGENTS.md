# ElektroCaster Agent Rules

## Project summary

ElektroCaster is a complex hardware/firmware project for a partially 3D-printed electric guitar with a LED fretboard, touch-sensitive fretboard sensing, Kickup solenoid actuators, a MulEBow electromagnetic actuator, hexaphonic audio processing, many body controls, and an OLED display.

The project uses three microcontrollers:

- `software/firmwareHid`: Teensy 3.5 firmware for body controls and the OLED display.
- `software/firmwareCtl`: Teensy 4.1 main controller firmware for system state, LED fretboard, fretboard sensing, actuators, sequencers, MIDI, SD song storage, and board coordination.
- `software/firmwareAudio`: Teensy 4.1 audio firmware using the Teensy Audio Library for hexaphonic DSP and actuator audio/control outputs.

## Mandatory rules for future agent tasks

- Do not change pin assignments unless the user explicitly asks for that change.
- Do not change serial protocols, command IDs, payload formats, baud rates, or scaling unless the user explicitly asks for that change.
- Do not change realtime audio code, `AudioConnection` setup, or audio routing unless the user explicitly asks for that change.
- Do not change fretboard scanning, debounce thresholds, clock timing, or Kickup timing unless the user explicitly asks for that change.
- Refactoring tasks must be behavior-preserving unless the user explicitly requests behavior changes.
- Prefer small, reviewable pull requests over broad rewrites.
- Document uncertainty instead of inventing hardware details or protocol semantics.
- Do not edit `.ino`, `.cpp`, or `.h` files for documentation-only tasks.
- In every final response, list affected files and checks that were not run.

## Recommended workflow

1. Read this file before modifying files in this repository.
2. Check `docs/architecture.md`, `docs/protocols.md`, `docs/testing.md`, and, when present, `docs/pinout.md` and `docs/realtime.md` before touching firmware.
3. Preserve existing behavior first; document suggested logic changes separately if they are outside the requested scope.
4. For firmware changes, identify the affected board, serial links, pins, realtime timing, and hardware tests before implementation.
