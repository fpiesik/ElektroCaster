//
// Copyright (C) 2017 Thomas O. Fredericks, Sofian Audry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

//ADDED FOR COMPATIBILITY WITH WIRING
extern "C" {
  #include <stdlib.h>
}

#include "AsciiMassageParser.h"

AsciiMassageParser::AsciiMassageParser() {
  flush();
}

void AsciiMassageParser::flush() {
  MassageParser::flush();
  _nextIndex = 0;
  _parsingString = false;
  _buffer[MASSAGE_PARSER_BUFFERSIZE -1] = 0;
}

int8_t AsciiMassageParser::nextByte(bool* error) {
  int8_t v;
  _nextBlock(true, (uint8_t*)&v, sizeof(int8_t), error);

  return v;
}

int16_t AsciiMassageParser::nextInt(bool* error)
{
  int16_t v;
  _nextBlock(true, (uint8_t*)&v, sizeof(int16_t), error);

  return v;
}

int32_t AsciiMassageParser::nextLong(bool* error)
{
  int32_t v;
  _nextBlock(true, (uint8_t*)&v, sizeof(int32_t), error);

  return v;
}

float AsciiMassageParser::nextFloat(bool* error)
{
  double v;
  _nextBlock(false, (uint8_t*)&v, sizeof(double), error);

  return (float)v;
}

int AsciiMassageParser::nextString(char* destination, int bufferLength) {
  
  if ( bufferLength < 1 || !destination ) return 0;


  int i = 0;
  int limit = bufferLength-1;

  unsigned char* source = &_buffer[_nextIndex];

  while ( i < limit && *source ) {
    *destination = *source;
    destination++;
    source++;
    i++;
  }
  
  *destination = 0;

  _updateNextIndex();

  return i;
}

bool AsciiMassageParser::_decode(int streamByte)
{
  // Check if we've reached the end of the buffer.
  if (_messageSize >= (MASSAGE_PARSER_BUFFERSIZE -1))
  {

    flush();
    return false;
  }

  // Process byte.
  switch (streamByte)
  {
    case '\n': // LF
    case '\r': // CR
      if (_messageSize > 0) // only process this if we are *not* at beginning
      {
        if (_buffer[_messageSize-1] != 0)
          _store(0);

        // Position _nextIndex after command address string.
        _nextIndex = 0;
        _updateNextIndex();

        return true;
      }

      flush();
      break;
    case 0 : // this should never happen
    case ' ':

     if ( _parsingString ) {
          _store(' ');
      } else {
          // Put null character instead of space to easily use atoi()/atof() functions.
          if (_messageSize > 0 && _buffer[_messageSize-1] != 0)
          {
            _store(0);
          }
      }
      break;
    case 34 : // " character
      _parsingString = !_parsingString;
      break;
    default: // caught a non-reserved character
      _store(streamByte);
  }

  return false;
}

bool AsciiMassageParser::_updateNextIndex()
{
  while (_buffer[_nextIndex] != 0) _nextIndex++; // move up till next null character (spaces are converted to null characters)
  _nextIndex++; // skip the null character
  return (_nextIndex < _messageSize);
}

bool AsciiMassageParser::_hasNext() const {
  return (_nextIndex < _messageSize);
}

void AsciiMassageParser::_nextBlock(bool isInteger, uint8_t* value, size_t n, bool* error)
{
  // Check for errors.
  bool err = !_hasNext();
  if (err)
    memset(value, 0, n); // set to zero (default)

  if (error)
    *error = err;

  // Process data.
  if (!err)
  {
    // Switch integer vs real.
    if (isInteger)
    {
      long val = strtol((const char*) &_buffer[_nextIndex], 0, 10);
      memcpy(value, &val, n);
    }
    else
    {
      double  val = strtod((const char*) &_buffer[_nextIndex], 0);
      memcpy(value, &val, n);
    }

    _updateNextIndex();
  }

}
