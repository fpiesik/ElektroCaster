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

#ifndef MassageParser_h
#define MassageParser_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef MASSAGE_PARSER_BUFFERSIZE
/**
 * Max. size of message buffer. Can be predefined before including MassageParser.h
 * to increase/decrease size.
 */
#define MASSAGE_PARSER_BUFFERSIZE 256
#endif


/// Main MassageParser abstract class.
class MassageParser
{
public:
  typedef void (*callbackFunction)(void);

    /**
   * Reads one char, flushing previous message if needed.
   * Returns true if new message has been parsed.
   * Optional callback function will be called if new message has arrived.
   */
private:
  virtual bool parse(int data, callbackFunction callback = 0)
  {
    // Flush if needed.
    if ( _needToFlush) {
      flush();
    }

    // Read stream.
    if ( _decode(data) ) {
      _needToFlush = true;

      // Call optional callback.
      if (callback)
        callback();

      return true;
    }

    return false;
  }

public:

  /// Constructor.
  MassageParser( ) {
    flush();
  }

  // Virtual destructor.
  virtual ~MassageParser() {}



  /**
   * Reads all elements of a stream, flushing previous message if needed.
   * Returns true if new message has been parsed.
   * Optional callback function will be called if new message has arrived.
   */
  virtual bool parseStream(Stream* stream, callbackFunction callback = 0) {
     while ( stream->available() ) {
        if ( parse( stream->read() , callback ) ) {
            return true;
        }
    }
    return false;
  }

  /// Flushes current message in buffer (if any).
  virtual void flush() {
    _needToFlush = false;
    _messageSize = 0;
  }

  /// Return true if current message matches "address".
  virtual bool fullMatch(const char* address)
  {
    // Verity if address matches beginning of buffer.
    bool matches = (strcmp((const char*) _buffer, address) == 0);
    return matches;
  }

  /**
	 * If current message matches "address", calls function "callback" and returns true;
	 * (otherwise returns false).
	 */
  virtual bool dispatch(const char* address, callbackFunction callback) {
    // Verify if address matches beginning of buffer.
    if ( fullMatch(address) ) {
      callback();
      return true;
    }
    return false;
  }

  /// Reads next byte.
  virtual int8_t nextByte(bool* error=0) = 0;

  /// Reads next int.
  virtual int16_t nextInt(bool* error=0) = 0;

  /// Reads next long.
  virtual int32_t nextLong(bool* error=0) = 0;

  /// Reads next float.
  virtual float nextFloat(bool* error=0) = 0;

  /// Reads next string.
  virtual int nextString(char* receivedString, int bufferLength) = 0;

protected:
  // Decodes a single value read from the serial stream (returns true if message is complete).
  virtual bool _decode(int serialByte) = 0;

/*
  void parseStream(Stream* stream, callbackFunction callback)
  {
  	  while ( stream->available() ) {
    // PARSE INPUT AND EXECUTRE massageReceived IF A COMPLETE MASSAGE IS RECEIVED
    		parse( stream->read() , callback );
  	  }
  }
*/

  // Writes single byte to buffer (returns false if buffer is full and cannot be written to).
  bool _store(uint8_t value)
  {
    if (_messageSize >= MASSAGE_PARSER_BUFFERSIZE)
      return false;

    _buffer[_messageSize++] = value;
    return true;
  }

	// True if message has ended (internal use).
  bool _needToFlush;

  // Current size of message in buffer.
  size_t _messageSize;

  // Buffer that holds the data for current message to be sent.
  uint8_t _buffer[MASSAGE_PARSER_BUFFERSIZE];
};






#endif
