# ElektroCaster pinout notes

This document records pin assignments inferable from firmware source. It does not change pin values. Hardware labels and board connector names are open unless explicitly shown in code.

## Board-level pin ownership

| Firmware | Pin / hardware area | Source of truth in firmware | Notes |
| --- | --- | --- | --- |
| `software/firmwareHid` | Body controls, encoders, OLED I2C | `software/firmwareHid/HardwareConfig.h`, `software/firmwareHid/firmwareHid.ino` | OLED constructor uses hardware I2C with `U8X8_PIN_NONE`; exact SDA/SCL board pins are implied by Teensy hardware I2C rather than listed here. |
| `software/firmwareCtl` | LED data, fret contacts, string sense pins, Kickup outputs | `software/firmwareCtl/HardwareConfig.h`, `software/firmwareCtl/firmwareCtl.ino` | LED matrix order is encoded in `led_pixPos`; fret and string scan semantics are timing-sensitive. |
| `software/firmwareAudio` | TDM audio channels and codec/audio objects | `software/firmwareAudio/firmwareAudio.ino`, `software/firmwareAudio/0Setup.ino` | Physical codec connector labels are not documented in source. |

## `software/firmwareHid`

### Encoders

| Encoder | Pins |
| --- | --- |
| `enc0` | `16`, `15` |
| `enc1` | `3`, `2` |
| `enc2` | `6`, `5` |
| `enc3` | `9`, `8` |
| `enc4` | `12`, `11` |
| `enc5` | `26`, `24` |
| `enc6` | `29`, `28` |
| `enc7` | `45`, `44` |

Note: `nEnc` is declared as `8`; eight encoders are instantiated and sent.

### Analog inputs

`aPin[22]`:

| Index | Pin | Notes |
| ---: | --- | --- |
| 0 | `31` | Analog control; exact hardware function open. |
| 1 | `32` | Analog control; exact hardware function open. |
| 2 | `33` | Analog control; exact hardware function open. |
| 3 | `34` | Analog control; exact hardware function open. |
| 4 | `35` | Analog control; exact hardware function open. |
| 5 | `36` | Analog control; exact hardware function open. |
| 6 | `37` | Analog control; exact hardware function open. |
| 7 | `38` | Analog control; exact hardware function open. |
| 8 | `39` | Analog control; exact hardware function open. |
| 9 | `A21` | Analog control; exact hardware function open. |
| 10 | `A22` | Analog control; exact hardware function open. |
| 11 | `14` | Analog control; exact hardware function open. |
| 12 | `A26` | Analog control; exact hardware function open. |
| 13 | `A25` | Analog control; exact hardware function open. |
| 14 | `A11` | Analog control; exact hardware function open. |
| 15 | `A10` | Analog control; exact hardware function open. |
| 16 | `A24` | Analog control; exact hardware function open. |
| 17 | `A23` | Analog control; exact hardware function open. |
| 18 | `A9` | Analog control; exact hardware function open. |
| 19 | `A8` | Rotary switch resistor ladder input. |
| 20 | `A7` | Rotary switch resistor ladder input. |
| 21 | `A6` | Rotary switch resistor ladder input. |

### Digital inputs

`dPin[19]`:

| Index | Pin | Notes |
| ---: | --- | --- |
| 0 | `17` | Digital control; exact hardware function open. |
| 1 | `13` | Digital control; exact hardware function open. |
| 2 | `51` | Digital control; exact hardware function open. |
| 3 | `52` | Digital control; exact hardware function open. |
| 4 | `53` | Digital control; exact hardware function open. |
| 5 | `42` | Digital control; exact hardware function open. |
| 6 | `41` | Digital control; exact hardware function open. |
| 7 | `40` | Digital control; exact hardware function open. |
| 8 | `56` | Digital control; exact hardware function open. |
| 9 | `57` | Digital control; exact hardware function open. |
| 10 | `43` | Digital control; exact hardware function open. |
| 11 | `54` | Digital control; exact hardware function open. |
| 12 | `55` | Digital control; exact hardware function open. |
| 13 | `4` | Digital control; exact hardware function open. |
| 14 | `7` | Digital control; exact hardware function open. |
| 15 | `10` | Digital control; exact hardware function open. |
| 16 | `25` | Digital control; exact hardware function open. |
| 17 | `27` | Digital control; exact hardware function open. |
| 18 | `30` | Digital control; exact hardware function open. |

## `software/firmwareCtl`

### Serial ports

| Port | Baud | Inferred use |
| --- | ---: | --- |
| `Serial` | 115200 | Debug USB serial. |
| `Serial1` | 250000 | Audio board link. |
| `Serial7` | 115200 | HID board link. |
| `Serial8` | MIDI library instance | Hardware MIDI. |

### LED fretboard

| Symbol | Value | Notes |
| --- | --- | --- |
| `LED_PIN` | `14` | WS2812Serial/FastLED data pin. |
| `NUMPIXELS` | `150` | Total LED count. |
| `nLedFrets` | `25` | LED fret positions per string. |

The physical LED matrix mapping is encoded in `led_pixPos[6][25]`. Exact physical orientation should be verified against hardware.

### Fretboard contact sensing

| Symbol | Values | Notes |
| --- | --- | --- |
| `nFrets` | `21` | Fret contact count used for scanning. |
| `frtPins` | `{13,33,2,3,4,5,6,7,8,9,10,11,12,24,25,26,27,30,31,32,17,16}` | Contains 22 entries while `nFrets` is 21; open issue to verify. |
| `strPins` | `{18,19,20,21,22,23}` | Six string sense inputs. |

Open: `strSnsPins` is also declared as `{2,3,4,5,6,7}` in the string definitions section, but the fretboard scanner uses `strPins`.

### Kickup

| String index | Pin |
| ---: | ---: |
| 0 | `41` |
| 1 | `40` |
| 2 | `39` |
| 3 | `38` |
| 4 | `37` |
| 5 | `36` |

## `software/firmwareAudio`

### Serial ports

| Port | Baud | Inferred use |
| --- | ---: | --- |
| `Serial` | 115200 | Debug USB serial. |
| `Serial1` | 250000 | CTL board link. |

### TDM audio channel mappings

| Array | Values | Notes |
| --- | --- | --- |
| `strAIn[6]` | `{10,8,6,4,2,0}` | Audio input channels of the strings. |
| `strAOut[6]` | `{10,8,6,4,2,0}` | Output channels of the string coils. |

Open: Physical codec channel labels and connector wiring are not documented in source.
