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

#include "AsciiMassagePacker.h"

AsciiMassagePacker::AsciiMassagePacker() {
    MassagePacker::flush();
}



void AsciiMassagePacker::beginPacket(const char* address)
{
  MassagePacker::flush();
  
  while(*address) write(*address++); // print(string) does not work Duemilanove with Diecimila

}

void AsciiMassagePacker::addByte(uint8_t value)
{
  addLong(value);
}

void AsciiMassagePacker::addInt(int16_t value)
{
  addLong(value);
}

void AsciiMassagePacker::addLong(int32_t value)
{
  write(' ');
  print(value);
}

void AsciiMassagePacker::addFloat(float value)
{
  write(' ');
  print(value);
}

void AsciiMassagePacker::addString(const char * value)
{
  write(' ');
  write(34); // " char
  print(value);
  write(34); 
}

void AsciiMassagePacker::endPacket()
{
  write('\n');
}


size_t AsciiMassagePacker::write(uint8_t data) {
	MassagePacker::_store(data);
	
  return 1;
}
