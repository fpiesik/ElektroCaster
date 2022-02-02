/* * Copyright (C) 2017 Thomas O. Fredericks, Sofian Audry
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MassagePacker_h
#define MassagePacker_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef MASSAGE_PACKER_BUFFERSIZE
/**
 * Max. size of message buffer. Can be predefined before including MassagePacker.h
 * to increase/decrease size.
 */
#define MASSAGE_PACKER_BUFFERSIZE 256
#endif


/// Main MassagePacker abstract class.
class MassagePacker
{
public:
  /// Constructor.
  MassagePacker() {
    flush();
  }

  // Virtual destructor.
  virtual ~MassagePacker() {}

  /// Flushes current message in buffer (if any).
  void flush() {
    _messageSize = 0;
  }

  /// Returns size of buffer.
  size_t size() const {
    return _messageSize;
  }

  // Returns a pointer to the buffer.
  const uint8_t* buffer() const {
    return _buffer;
  }

  /// Begins the sending of a message.
  virtual void beginPacket(const char* address) = 0;

  /// Adds a byte.
  virtual void addByte(uint8_t value) = 0;

  /// Adds an int.
  virtual void addInt(int16_t value) = 0;

  /// Adds a long.
  virtual void addLong(int32_t value) = 0;

  /// Adds a float.
  virtual void addFloat(float value) = 0;

    /// Adds a float.
  virtual void addString(const char* value) = 0;

  /// Ends the sending of a message.
  virtual void endPacket() = 0;

  /// Ends the sending of a message and sends it through a Stream
  virtual void streamPacket(Stream* stream) {
  	endPacket();
  	stream->write( buffer(), size() );
  }

  /// Create a packet with no arguments.
  virtual void packEmpty(const char *address)
  {
    beginPacket(address);
    endPacket();
  }

  /// Create a packet with a single byte value.
  virtual void packOneByte(const char *address, uint8_t value)
  {
    beginPacket(address);
    addByte(value);
    endPacket();
  }

  /// Create a packet with a single int value.
  virtual void packOneInt(const char *address, int16_t value)
  {
    beginPacket(address);
    addInt(value);
    endPacket();
  }

  /// Create a packet with a single long value.
  virtual void packOneLong(const char *address, int32_t value)
  {
    beginPacket(address);
    addLong(value);
    endPacket();
  }

  /// Create a packet with a single float value.
  virtual void packOneFloat(const char *address, float value)
  {
    beginPacket(address);
    addFloat(value);
    endPacket();
  }

    /// Create a packet with a single string value.
  virtual void packOneString(const char *address, const char* value)
  {
    beginPacket(address);
    addString(value);
    endPacket();
  }

  /// Stream a packet with no arguments.
  virtual void streamEmpty(Stream* stream, const char *address)
  {
    beginPacket(address);
    streamPacket(stream);
  }

  /// Stream a packet with a single byte value.
  virtual void streamOneByte(Stream* stream, const char *address, uint8_t value)
  {
    beginPacket(address);
    addByte(value);
    streamPacket(stream);
  }

  /// Stream a packet with a single int value.
  virtual void streamOneInt(Stream* stream, const char *address, int16_t value)
  {
    beginPacket(address);
    addInt(value);
    streamPacket(stream);
  }

  /// Stream a packet with a single long value.
  virtual void streamOneLong(Stream* stream, const char *address, int32_t value)
  {
    beginPacket(address);
    addLong(value);
    streamPacket(stream);
  }

  /// Stream a packet with a single float value.
  virtual void streamOneFloat(Stream* stream, const char *address, float value)
  {
    beginPacket(address);
    addFloat(value);
    streamPacket(stream);
  }

    /// Stream a packet with a single string value.
  virtual void streamOneString(Stream* stream, const char *address, const char * s)
  {
    beginPacket(address);
    addString(s);
    streamPacket(stream);
  }


protected:
  // Writes single byte to buffer (returns false if buffer is full and cannot be written to).
  bool _store(uint8_t value)
  {
    if (_messageSize >= MASSAGE_PACKER_BUFFERSIZE)
      return false;

    _buffer[_messageSize++] = value;
    return true;
  }

  // Current size of message in buffer.
  size_t _messageSize;

  // Buffer that holds the data for current message to be sent.
  uint8_t _buffer[MASSAGE_PACKER_BUFFERSIZE];
};




#endif
