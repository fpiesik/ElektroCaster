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
