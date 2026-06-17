# ElektroCaster realtime and timing notes

This document records timing values inferable from firmware source. It does not change timing behavior.

## `software/firmwareCtl`

### LED fretboard

| Symbol | Value | Meaning |
| --- | ---: | --- |
| `fled_frameInt` | `50` ms | LED fretboard frame/update interval. |

### Fretboard scanning and debounce

| Symbol | Value | Meaning |
| --- | ---: | --- |
| `fretMaskT` | `50` ms | Minimum time before another string press on the same string is detected. |
| `strBncs` | `500` samples | Same-sample count threshold for normal string press detection. |
| `strBncsP` | `5000` samples | Same-sample count threshold intended for pattern section detection; related code appears commented/disabled. |

Notes:

- `readFretboard()` is called from the main loop.
- The scanner switches fret pins between output/input modes depending on sensing mode.
- Fretboard scan behavior is protected and should not be changed without explicit request and hardware tests.

### Kickup timing

| Symbol | Value | Meaning |
| --- | ---: | --- |
| `kickDur[6]` | `{5,5,5,5,5,5}` ms | Pulse duration per string. |
| `kCueInt` | `3` ms | Interval used to stagger queued kicks. |

Notes:

- `cueKicks()` calls `kickOff()` and then triggers at most one queued string per cue interval.
- This likely prevents simultaneous solenoid current spikes.

### Display update

| Symbol | Value | Meaning |
| --- | ---: | --- |
| `disp_frameInt` | `200` ms | Display update interval. |

Note: `updDisplay()` is currently called from `updIntClock()`, so display refresh is coupled to internal clock processing when `extClk == 0`.

### Clock timing

| Item | Value / formula | Meaning |
| --- | --- | --- |
| Internal PPQN basis | `24` | `bpm2Micros()` divides by 24. |
| `bpm2Micros(val)` | `60000000 / val / 24` | Microseconds per MIDI-clock-style tick. |
| `bpm2Millis(val)` | `60000 / val / 24` | Milliseconds equivalent; comment says microseconds but function name says millis. |
| Default `bpm` | `90` | Main controller default BPM. |

Clock-derived state:

- `pulse = mClock / 24`
- `bar = mClock / 96`
- `syncPnt = mClock / syncInt`

Open: `syncInt` initialization should be verified before any clock refactor.

## `software/firmwareHid`

| Symbol / call | Value | Meaning |
| --- | ---: | --- |
| `waitS` | `50` microseconds | Delay inserted between HID protocol bytes. |
| `analogReadAveraging(200)` | `200` samples | Analog input averaging setting. |
| startup display delay | `2000` ms | Delay after `u8g2.begin()`. |

## `software/firmwareAudio`

| Symbol | Value | Meaning |
| --- | ---: | --- |
| `ctlInt` | `10` ms | Control loop interval placeholder/check interval. |
| `nFrqInt` | `20` ms | Pitch analysis/send interval. |
| `peakInt` | `20` ms | RMS/amplitude analysis/send interval. |
| `waitS` | `100` microseconds | Delay used between bytes in some Audio-to-CTL senders. |
| `AudioMemory` | `2000` blocks | Teensy Audio memory allocation. |
| setup delay | `1000` ms | Delay before audio initialization. |

Notes:

- Pitch analysis resumes/checks all six `AudioAnalyzeMonoFrequency` instances every `nFrqInt` interval.
- RMS analysis checks all six `AudioAnalyzeRMS` instances every `peakInt` interval.
- Debug processor/memory usage printing exists but is disabled behind `if(0)`.

## Protected timing areas

Do not change these without explicit user instruction and hardware validation:

- LED frame interval and LED update path.
- Fretboard pin scanning mode, debounce counters, and mask time.
- Kickup pulse duration and queue interval.
- Internal clock tick calculation and pattern sync behavior.
- Audio analysis intervals and Audio Library routing.
- Serial byte spacing unless the protocol is intentionally redesigned.
