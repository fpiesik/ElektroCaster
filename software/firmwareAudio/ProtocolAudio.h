#pragma once

#include <Arduino.h>

constexpr byte AUDIO_CMD_BASE = 200;
constexpr byte AUDIO_CMD_MAX = 255;

// CTL -> Audio command bytes.
constexpr byte AUDIO_CMD_TRIG_ENV = 200;
constexpr byte AUDIO_CMD_STR_FRET = 201;
constexpr byte AUDIO_CMD_OP_MODE = 202;
constexpr byte AUDIO_CMD_DISP_MODE = 203;
constexpr byte AUDIO_CMD_KICK_MODE = 204;
constexpr byte AUDIO_CMD_BOW_MODE = 205;
constexpr byte AUDIO_CMD_BOW_ON = 206;
constexpr byte AUDIO_CMD_ENV_1 = 207;
constexpr byte AUDIO_CMD_ENV_2 = 208;
constexpr byte AUDIO_CMD_FILTER = 209;
constexpr byte AUDIO_CMD_MANUAL_ENV = 210;
constexpr byte AUDIO_CMD_VOLUME = 211;
constexpr byte AUDIO_CMD_STR_GAIN = 212;
constexpr byte AUDIO_CMD_FX = 215;
constexpr byte AUDIO_CMD_LFO_1 = 216;
constexpr byte AUDIO_CMD_BPM = 217;
constexpr byte AUDIO_CMD_MIDI_CC = 218;

// Audio -> CTL status command bytes.
constexpr byte AUDIO_STATUS_NOTE = 201;
constexpr byte AUDIO_STATUS_CC = 205;
constexpr byte AUDIO_STATUS_STR_PITCH = 206;
constexpr byte AUDIO_STATUS_STR_AMPLITUDE = 207;

constexpr byte audioIncoming(byte command) {
  return command - AUDIO_CMD_BASE;
}
