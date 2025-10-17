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
