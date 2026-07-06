#include "ProtocolAudio.h"
#include "ProtocolHid.h"

namespace {
constexpr unsigned long AUDIO_STATUS_PACKET_TIMEOUT_MS = 10;
constexpr unsigned long HID_PACKET_TIMEOUT_MS = 20;
constexpr byte AUDIO_STATUS_MAX_PAYLOAD = 3;

// Audio -> CTL status payload lengths. These preserve the existing byte protocol:
// status byte AUDIO_STATUS_* followed by exactly this many payload bytes.
byte audioStatusPayloadLength(byte command) {
  switch (command) {
    case AUDIO_STATUS_STR_PITCH: return 3;
    case AUDIO_STATUS_STR_AMPLITUDE: return 2;
    default: return 0;
  }
}

void resetAudioStatusPacket(byte &command, byte &payloadIndex, byte &payloadLength) {
  command = 0;
  payloadIndex = 0;
  payloadLength = 0;
}

void handleAudioStatusPacket(byte command, const byte payload[]) {
  int incoming = audioIncoming(command);

  if (incoming == audioIncoming(AUDIO_STATUS_STR_PITCH)) {
    strP[payload[0]] = payload[1] + payload[2] / 100.0;
  }

  if (incoming == audioIncoming(AUDIO_STATUS_STR_AMPLITUDE)) {
    strA[payload[0]] = payload[1] / 100.0;
  }
}

void resetHidPacket(int &incoming, bool &waitingForPayload) {
  incoming = -1;
  waitingForPayload = false;
}

void handleHidPacket(int incoming, byte val) {
  if (incoming < HID_ANALOG_OFFSET) rcvHidD(incoming, val);
  if (incoming >= HID_ANALOG_OFFSET && incoming < HID_ROTARY_OFFSET) rcvHidA(incoming - HID_ANALOG_OFFSET, val);
  if (incoming >= HID_ROTARY_OFFSET && incoming < HID_ENCODER_OFFSET) rcvHidR(incoming - HID_ROTARY_OFFSET, val);
  if (incoming >= HID_ENCODER_OFFSET && incoming < HID_ENCODER_END_OFFSET) rcvHidE(incoming - HID_ENCODER_OFFSET, val - HID_ENCODER_CENTER);
}
}

void serialEvent1(){
  static byte command = 0;
  static byte payload[AUDIO_STATUS_MAX_PAYLOAD];
  static byte payloadIndex = 0;
  static byte payloadLength = 0;
  static unsigned long packetStartedAt = 0;

  if (payloadLength > 0 && millis() - packetStartedAt > AUDIO_STATUS_PACKET_TIMEOUT_MS) {
    resetAudioStatusPacket(command, payloadIndex, payloadLength);
  }

  while (Serial1.available() > 0) {
    byte sbyte = Serial1.read();

    if (sbyte >= AUDIO_CMD_BASE && sbyte <= AUDIO_CMD_MAX) {
      command = sbyte;
      payloadIndex = 0;
      payloadLength = audioStatusPayloadLength(command);
      packetStartedAt = millis();
      if (payloadLength == 0 || payloadLength > AUDIO_STATUS_MAX_PAYLOAD) {
        resetAudioStatusPacket(command, payloadIndex, payloadLength);
      }
      continue;
    }

    if (payloadLength == 0) {
      continue;
    }

    payload[payloadIndex++] = sbyte;
    if (payloadIndex >= payloadLength) {
      handleAudioStatusPacket(command, payload);
      resetAudioStatusPacket(command, payloadIndex, payloadLength);
    }
  }
}

void serialEvent7(){
  static int incoming = -1;
  static bool waitingForPayload = false;
  static unsigned long packetStartedAt = 0;

  if (waitingForPayload && millis() - packetStartedAt > HID_PACKET_TIMEOUT_MS) {
    resetHidPacket(incoming, waitingForPayload);
  }

  while (Serial7.available() > 0) {
    byte serbyte = Serial7.read();

    if (serbyte >= HID_CMD_BASE && serbyte <= HID_CMD_MAX) {
      incoming = hidIncoming(serbyte);
      waitingForPayload = true;
      packetStartedAt = millis();
      continue;
    }

    if (serbyte == HID_FRAME_END) {
      resetHidPacket(incoming, waitingForPayload);
      continue;
    }

    if (!waitingForPayload || incoming < 0) {
      continue;
    }

    handleHidPacket(incoming, serbyte);
    resetHidPacket(incoming, waitingForPayload);
  }
}
