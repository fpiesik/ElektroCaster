#pragma once

#include <Arduino.h>

constexpr byte HID_CMD_BASE = 201;
constexpr byte HID_CMD_MAX = 254;
constexpr byte HID_FRAME_END = 255;

constexpr byte HID_DIGITAL_OFFSET = 0;
constexpr byte HID_ANALOG_OFFSET = 19;
constexpr byte HID_ROTARY_OFFSET = 38;
constexpr byte HID_ENCODER_OFFSET = 41;
constexpr byte HID_ENCODER_END_OFFSET = 49;
constexpr byte HID_ENCODER_CENTER = 100;
constexpr byte HID_ANALOG_MAX_VALUE = 200;

constexpr byte hidIncoming(byte command) {
  return command - HID_CMD_BASE;
}
