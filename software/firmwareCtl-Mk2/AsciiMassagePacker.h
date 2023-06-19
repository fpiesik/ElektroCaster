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

#ifndef AsciiMassagePacker_h
#define AsciiMassagePacker_h


#include "Massage/MassagePacker.h"
#include "Print.h"

/**
 AsciiMassagePacker class
 */
class AsciiMassagePacker : public MassagePacker, private Print
{
public:
  /**
 Constructor
 */
  AsciiMassagePacker();
  /**
  Begins the sending of a message.
   */
  virtual void beginPacket(const char* address);

  /**
  Adds a byte.
   */
  virtual void addByte(uint8_t value);

  /**
  Adds an int.
   */
  virtual void addInt(int16_t value);

    /**
  Adds a long.
   */
  virtual void addLong(int32_t value);

   /**
  Adds a float.
   */
  virtual void addFloat(float value);

   /**
  Adds a string.
   */
  virtual void addString(const char * value);

  /**
  [[Deprecated]] Ends a packet.
  */
  virtual void endPacket();



  // REQUIRED BY PRINT, BUT SHOULD NEVER BE USED EXTERNALLY !!!
private:
  virtual size_t write(uint8_t) ;
};




#endif
