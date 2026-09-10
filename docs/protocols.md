# ElektroCaster serial protocols

This document records currently observed serial protocols. It is based on repository source code only. Where semantics are unclear, they are marked as open.

## Serial links and baud rates

| Link | Sender-side port | Receiver-side port | Baud | Notes |
| --- | --- | --- | --- | --- |
| HID -> CTL | `firmwareHid` `Serial6` | `firmwareCtl` `Serial7` | 115200 | HID controls to main controller. |
| CTL -> HID display | `firmwareCtl` `Serial7` | `firmwareHid` `Serial6` | 115200 | AsciiMassage display drawing commands. Same physical link is likely bidirectional. |
| CTL -> Audio | `firmwareCtl` `Serial1` | `firmwareAudio` `Serial1` | 250000 | Raw byte command protocol. |
| Audio -> CTL | `firmwareAudio` `Serial1` | `firmwareCtl` `Serial1` | 250000 | Raw byte status protocol. Same physical link is likely bidirectional. |

Open question: physical wiring and electrical directionality are not documented in the repository.

## Protocol byte-space summary

| Direction | Transport | Framing style | Command / packet namespace | Payload style | Receiver notes |
| --- | --- | --- | --- | --- | --- |
| HID -> CTL | Raw bytes on HID/CTL serial link | Command/index byte followed by value byte; HID also sends `255` after `sendAllNew()` | Command bytes are offset from HID indexes using `+ 201`; CTL subtracts `201` before dispatch | One value byte per recognized input event | CTL uses non-blocking packet assembly with a short timeout; no checksum or versioning identified. |
| CTL -> HID display | AsciiMassage on HID/CTL serial link | Named AsciiMassage packets | Text packet names such as `str`, `int`, `frm`, `buf` | Packet-specific byte/int/long/string fields | HID delegates parsing/framing to AsciiMassage and executes U8g2 drawing calls. |
| CTL -> Audio | Raw bytes on CTL/Audio serial link | Command byte `200..255`, then command-specific payload | Shared audio command constants; Audio decodes `command - 200` | One or more bytes; most scaled values are intended to stay `<= 199` | Audio uses non-blocking packet assembly with a short timeout; no checksum or length byte identified. |
| Audio -> CTL | Raw bytes on CTL/Audio serial link | Status command byte, then status-specific payload | Audio status constants reuse some numeric IDs from the opposite direction | Pitch/amplitude/status bytes | CTL decodes by direction, so numeric reuse is only safe because links are directional at parser level. |

## HID -> CTL protocol

Implemented in:

- `software/firmwareHid/ProtocolHid.h`
- `software/firmwareHid/send.ino`
- `software/firmwareCtl/ProtocolHid.h`
- `software/firmwareCtl/13_serialEvents.ino`

General form: a command/index byte followed by one value byte. HID also emits `255` as a frame/end marker; CTL resets the current HID packet state when it receives this marker.

Constants inferred from code:

- `nDigital = 19`
- `nAnalog = 22`
- Rotary switch analog source indices are `19..21`, sent separately as rotary states instead of normal analog values.

| HID data type | Sender command byte | CTL decoded range | Payload | Value range / scaling |
| --- | --- | --- | --- | --- |
| Digital input | `idx + 201` | `incoming < 19` | 1 byte `val` | Raw digital state. |
| Analog input | `idx + nDigital + 201` | `incoming >= 19 && incoming < 38` | 1 byte `val` | `analogRead` mapped from `0..1023` to `0..200`; indices `19..21` are excluded. |
| Rotary switch | `idx + nDigital + 19 + 201` | `incoming >= 38 && incoming < 41` | 1 byte `val` | Rotary state index, likely `0..11`. |
| Encoder delta | `idx + nDigital + 22 + 201` | `incoming >= 41 && incoming < 49` | 1 byte `val` | Delta encoded as `delta + 100`; CTL subtracts `100`. |
| Frame/end marker | `255` | Packet-state reset marker | none | Sent by HID after `sendAllNew()`; CTL resets any pending HID packet state. |

Open/incomplete:

- `nEnc` is declared as `8`; eight encoder instances are instantiated and the protocol range supports encoder indices `0..7` through decoded values `41..48`.
- CTL uses a 20 ms timeout while assembling HID command/value pairs.
- No checksum or sequence number was identified.

## CTL -> HID display protocol

Implemented in:

- `software/firmwareCtl/03_display.ino`
- `software/firmwareHid/receive.ino`

Transport: AsciiMassage packets over the CTL/HID serial link.

Known packet names and payloads:

| Packet | Payload | Meaning |
| --- | --- | --- |
| `str` | `byte x`, `byte y`, `string text` | Draw string. |
| `int` | `byte x`, `byte y`, `int val` | Draw integer. |
| `lng` | `byte x`, `byte y`, `long val` | Draw long integer. |
| `frm` | `byte xp`, `byte yp`, `byte xs`, `byte ys` | Draw frame. |
| `box` | `byte xp`, `byte yp`, `byte xs`, `byte ys` | Draw box. |
| `rfr` | `byte xp`, `byte yp`, `byte xs`, `byte ys`, `byte r` | Draw rounded frame. |
| `rbx` | `byte xp`, `byte yp`, `byte xs`, `byte ys`, `byte r` | Draw rounded box. |
| `cir` | `byte x`, `byte y`, `byte r` | Draw circle. |
| `dis` | `byte x`, `byte y`, `byte r` | Draw filled disc. |
| `pix` | `byte x`, `byte y` | Draw pixel. |
| `col` | `byte c` | Set draw color. |
| `lin` | `byte x0`, `byte y0`, `byte x1`, `byte y1` | Draw line. |
| `clr` | none | Clear buffer. |
| `buf` | none | Send buffer to display. |
| `tuner` | open | Handler is present but commented out. |

Open/incomplete:

- Coordinate and color ranges are not formally documented; likely U8g2 display coordinates for a 128x64 OLED.
- Error handling for malformed packets is delegated to AsciiMassage behavior.

## CTL -> Audio protocol

Implemented in:

- `software/firmwareCtl/ProtocolAudio.h`
- `software/firmwareCtl/2audio.ino`
- `software/firmwareAudio/ProtocolAudio.h`
- `software/firmwareAudio/yLoop.ino`

General form: first byte is a command ID in the range `200..255`. Audio computes `incoming = command - 200`. Payload length depends on command.

Known command IDs:

| Command byte | Audio `incoming` | Sender function | Payload | Observed receiver behavior / scaling |
| --- | ---: | --- | --- | --- |
| `200` | `0` | `sndTrigEnv(str, vel)` | `byte str`, `byte velocity` | Audio calls `trigEnv(str, velocity / 199.0)`. |
| `201` | `1` | `sndStrPrs(str, pitch, state)` | `byte str`, `byte pitch`, `byte state` | Audio calls `strFret(str, pitch, state)` and stores `strState[str] = state`. |
| `202` | `2` | not found in CTL sender file | one byte | Audio calls `chOpMode(data)` when byte `<=199`. Source uncertain. |
| `203` | `3` | not found in CTL sender file | one byte | Audio calls `chDispMode(data)` when byte `<=199`. Source uncertain. |
| `204` | `4` | not found in CTL sender file | one byte | Audio calls `chKickMode(data)` when byte `<=199`. Source uncertain. |
| `205` | `5` | `sndBowMode(mode)` | `byte mode` | Audio calls `chBowMode(mode)` when byte `<=199`. Note: Audio-to-CTL CC also uses byte `205` in another direction. |
| `206` | `6` | `sndBowOn(mode)` | `byte mode` | Audio sets `bowOn = mode` when byte `<=199`. Note: Audio-to-CTL pitch also uses byte `206` in reverse direction. |
| `207` | `7` | `sndEnv1(para, val)` | `byte para`, `byte scaledVal` | Audio scales `scaledVal / 199.0` with `sclEnvA[para]`, calls `chEnvA`. |
| `208` | `8` | `sndEnv2(para, val)` | `byte para`, `byte scaledVal` | Audio scales `scaledVal / 199.0` with `sclEnvF[para]`, calls `chEnvF`. |
| `209` | `9` | `sndFilter(para, val)` | `byte para`, `byte scaledVal` | Audio scales with `sclFilter[para]`, calls `chFilter`. |
| `210` | `10` | `sndManualEnv(str, state)` | `byte str`, `byte state` | Audio stores per-string manual envelope-decay override; while enabled, amplitude and filter decay keep sustain high regardless of decay fader value. |
| `211` | `11` | `sndVol(val)` | `byte scaledVal` | Audio computes `(scaledVal / 199.0)^2 * 2`, then `ampOut.gain(val + 0.0001)`. |
| `212` | `12` | `sndStrGain(str, val)` | `byte str`, `byte scaledVal` | Audio computes `scaledVal / 100.0`, calls `chngStrOutGain`. CTL sends `val * 2`. |
| `215` | `15` | not found in CTL sender file | `byte para`, `byte scaledVal` | Audio scales with `sclFX[para]`, calls `chFX`. Source uncertain. |
| `216` | `16` | `sndLfo1(para, val)` | `byte para`, `byte scaledVal` | Audio scales with `sclLfo1[para]`, applies `chLfo1` to all strings. |
| `217` | `17` | not found in CTL sender file | one byte | Audio stores BPM byte. Source uncertain. |
| `218` | `18` | not found in CTL sender file | `byte a`, `byte b` | Audio sends MIDI CC `3, b/2` only if `a == 2`. Source uncertain. |

Open/incomplete:

- Commands `202`, `203`, `204`, `215`, `217`, and `218` are handled by Audio but corresponding CTL sender functions were not identified in `2audio.ino`.
- Audio uses a 10 ms timeout while assembling CTL-to-Audio packets.
- No checksum, version, length byte, escaping, or full resynchronization strategy was identified beyond treating bytes `>= 200` as command/status bytes.
- Values above `199` are treated as command bytes, so payload values should remain `<=199` unless explicitly safe.

## Audio -> CTL protocol

Implemented in:

- `software/firmwareAudio/ProtocolAudio.h`
- `software/firmwareAudio/2ctl.ino`
- `software/firmwareCtl/ProtocolAudio.h`
- `software/firmwareCtl/13_serialEvents.ino`

General form: first byte is a command ID, CTL computes `incoming = command - 200`.

Known command IDs:

| Command byte | CTL `incoming` | Sender function | Payload | CTL receiver behavior |
| --- | ---: | --- | --- | --- |
| `201` | `1` | `sndNote(pitch, vel)` | `byte pitch`, `byte vel` | Audio sender exists, but CTL currently assigns no payload length for this status, so these packets are ignored by the non-blocking parser. |
| `205` | `5` | `sndCC()` | `byte cc`, `byte val` | Audio sender exists, but CTL currently assigns no payload length for this status, so these packets are ignored by the non-blocking parser. |
| `206` | `6` | `sndStrP()` | `byte string`, `byte integerPart`, `byte fractionalPart` | CTL stores `strP[string] = integerPart + fractionalPart / 100.0`. |
| `207` | `7` | `sndStrA()` | `byte string`, `byte amplitudePercent` | CTL stores `strA[string] = amplitudePercent / 100.0`. |

Open/incomplete:

- Audio sender functions for note/CC exist, but CTL currently ignores those status IDs because only pitch and amplitude are assigned receive payload lengths.
- CTL uses a 10 ms timeout while assembling Audio-to-CTL status packets.
- Same numeric command IDs are reused in opposite directions, which is acceptable on a bidirectional link only if direction is always clear.
