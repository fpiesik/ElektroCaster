class AsciiMassagePacker {
  
  int _messageSize;
  byte[] _buffer;
  String internalString = "";
  boolean ended = false;
  
   /// Returns size of buffer.
  int size()  {
    return internalString.length();
  }

  // Returns a reference to the buffer.
  byte[] buffer()  {
    if ( !ended ) println("Warning: getting the buffer of an unended packet!");
    return internalString.getBytes();
  }

  /// Begins the sending of a message.
  void beginPacket(String address) {
    internalString = address;
    ended = false;
  }

  /// Adds a byte.
  void addByte(int value) {
    value = constrain(value,0,255);
    internalString = internalString +" " + value;
  }

  /// Adds an int.
   void addInt(int value) {
    internalString = internalString +" " + value;
  }

  /// Adds a long.
   void addLong(long value) {
    internalString = internalString +" " + value;
  }

  /// Adds a float.
   void addFloat(float value) {
    internalString = internalString +" " + value;
  }

  /// Ends the sending of a message.
  void endPacket() {
    internalString += '\n';
    ended = true;
  }
  
  void streamPacket(Serial serial) {
    endPacket();
    serial.write( buffer() );
  }

  /// Create a packet with no arguments.
  void packEmpty(String address)
  {
    beginPacket(address);
    endPacket();
  }

  /// Create a packet with a single byte value.
  void packOneByte(String address, byte value)
  {
    beginPacket(address);
    addByte(value);
    endPacket();
  }

  /// Create a packet with a single int value.
   void packOneInt(String address, int value)
  {
    beginPacket(address);
    addInt(value);
    endPacket();
  }

  /// Create a packet with a single long value.
   void packOneLong(String address, long value)
  {
    beginPacket(address);
    addLong(value);
    endPacket();
  }

  /// Create a packet with a single float value.
   void packOneFloat(String address, float value)
  {
    beginPacket(address);
    addFloat(value);
    endPacket();
  }
  
  /// Create a streamet with no arguments.
  void streamEmpty(Serial serial, String address)
  {
    beginPacket(address);
    streamPacket(serial);
  }

  /// Create a streamet with a single byte value.
  void streamOneByte(Serial serial, String address, byte value)
  {
    beginPacket(address);
    addByte(value);
    streamPacket(serial);
  }

  /// Create a streamet with a single int value.
   void streamOneInt(Serial serial, String address, int value)
  {
    beginPacket(address);
    addInt(value);
    streamPacket(serial);
  }

  /// Create a streamet with a single long value.
   void streamOneLong(Serial serial, String address, long value)
  {
    beginPacket(address);
    addLong(value);
    streamPacket(serial);
  }

  /// Create a streamet with a single float value.
   void streamOneFloat(Serial serial, String address, float value)
  {
    beginPacket(address);
    addFloat(value);
    streamPacket(serial);
  }
  
}