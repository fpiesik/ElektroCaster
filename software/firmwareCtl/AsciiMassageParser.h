#ifndef AsciiMassageParser_h
#define AsciiMassageParser_h


#include "Massage/MassageParser.h"


/// Main AsciiMassageParser class.
class AsciiMassageParser : public MassageParser
{
public:
  /// Constructor.
  AsciiMassageParser();

  /// Reads next byte.
  virtual int8_t nextByte(bool* error=0);

  /// Reads next int.
  virtual int16_t nextInt(bool* error=0);

  /// Reads next long.
  virtual int32_t nextLong(bool* error=0);

  /// Reads next float.
  virtual float nextFloat(bool* error=0);

  // Reads next string.
  virtual int nextString(char* receivedString, int bufferLength);

  /// Flushes current message in buffer (if any).
  virtual void flush();

protected:
  /// Decode sa single value read from the serial stream.
  /// Returns true if a massage is terminated.
  virtual bool _decode(int serialByte);

private:
  // Moves nextIndex to the next token.
  bool _updateNextIndex();

  // Returns true iff it is still possible to call next*().
  bool _hasNext() const;

  bool _parsingString;

  // Helper function to read next value.
  void _nextBlock(bool isInteger, uint8_t* value, size_t n, bool* error);

	// Keeps track of next index where to store data in buffer.
  size_t _nextIndex;


};




#endif
