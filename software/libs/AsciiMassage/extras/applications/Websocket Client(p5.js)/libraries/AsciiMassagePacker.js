function AsciiMassagePacker() {

  this._messageSize;
  this._internalString = "";
  this._ended = false;

  /// Returns size of buffer.
  this.size = function() {
    return this._internalString.length();
  }
 
   // Returns a reference to the buffer.
  this.buffer = function()  {
    if ( this.ended == false ) console.log("Warning: getting the buffer of an unended packet!");
    return this._internalString;
  }

  /// Begins the sending of a message.
  this.beginPacket = function(address) {
    this._internalString = address;
    this._ended = false;
  }

   /// Adds a byte.
  this.addByte = function(value) {
    this._internalString = this._internalString +" " + value;
  }

  /// Adds an int.
  this.addInt = function(value) {
    this._internalString = this._internalString +" " + value;
  }

  /// Adds a long.
   this.addLong = function(value) {
    this._internalString = this._internalString +" " + value;
  }

  /// Adds a float.
   this.addFloat = function(value) {
    this._internalString = this._internalString +" " + value;
  }

  /// Ends the sending of a message.
  this.endPacket = function() {
    this._internalString += '\n';
    this._ended = true;
  }

  /// Create a packet with no arguments.
  this.packEmpty = function(address)
  {
    this.beginPacket(address);
    this.endPacket();
  }

  /// Create a packet with a single byte value.
  this.packOneByte = function( address,  value)
  {
    this.beginPacket(address);
    this.addByte(value);
    this.endPacket();
  }

  /// Create a packet with a single int value.
   this.packOneInt = function( address,  value)
  {
    this.beginPacket(address);
    this.addInt(value);
    this.endPacket();
  }

  /// Create a packet with a single long value.
   this.packOneLong = function( address,  value)
  {
    this.beginPacket(address);
    this.addLong(value);
    this.endPacket();
  }

  /// Create a packet with a single float value.
   this.packOneFloat = function( address,  value)
  {
    this.beginPacket(address);
    this.addFloat(value);
    this.endPacket();
  }



}