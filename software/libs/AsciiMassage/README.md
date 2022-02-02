# AsciiMassage

*AsciiMassage* allows the sending and receiving of messages. 
More precisely, it is a microcontroller *massage* packer and parser for the ASCII format for any type of stream (Serial, UDP, etc).  A *massage* message always starts with an address string and is followed by a user defined number of elements (bytes, ints, longs, floats or strings).  The address string is used to route the message.

[This is the ASCII implementation](https://github.com/SofaPirate/AsciiMassage) of the [Massage API](https://github.com/SofaPirate/Massenger).

## GITHUB PAGE

https://github.com/SofaPirate/AsciiMassage

## DOCUMENTATION

The complete class documentation can be found in the "docs" folder or [online here](https://sofapirate.github.io/AsciiMassage/).




Receiving (parsing) a massage directly in loop()
--------------------------------------------------------

Add the library to the top of your code and instantiate an AsciiMassageParser called "inbound":
```cpp
#include <AsciiMassageParser.h>
AsciiMassageParser inbound;
```

Inside loop() parse the Serial stream with parse(). If parse() returns true, the massage is completed and ready.
```cpp
if ( inbound.parseStream( &Serial ) ) {
 // parse completed massage elements here.
}
```

This example parses the elements of a  massage that starts with the address "value" and that contains one long followed by one int:
```cpp
// Does the massage's address match "value"?
if ( inbound.fullMatch ("value") ) {
    // Get the first long.
    long ms = inbound.nextLong();
    // Get the next int.
    int an0 = inbound.nextInt();
}
```

The complete block of code is as follows:
```cpp
#include <AsciiMassageParser.h>
AsciiMassageParser inbound;

// [...]

void loop() {

   if ( inbound.parseStream( &Serial ) ) {
      // parse completed massage elements here.

      // Does the massage's address match "value"?
      if ( inbound.fullMatch ("value") ) {
          // Get the first long.
          long ms = inbound.nextLong();
          // Get the next int.
          int an0 = inbound.nextInt();
      }

   }

    // [...]
}
```

Receiving (parsing) a massage with a receive function
--------------------------------------------------------
Add the library to the top of your code and instantiate an AsciiMassageParser called "inbound":
```cpp
#include <AsciiMassageParser.h>
AsciiMassageParser inbound;
```

Add before loop() a function the will be called when a message is received
```cpp
void readMessage() {
   // parse completed massage elements here.
}
```

Inside loop() parse the Serial stream with parse() and run "readMessage" when a message is received
```cpp
inbound.parseStream( &Serial , readMessage );
```

The complete block of code is as follows:
```cpp
#include <AsciiMassageParser.h>
AsciiMassageParser inbound;

// [...]

void readMessage() {
    // parse completed massage elements here.

    // Does the massage's address match "value"?
    if ( inbound.fullMatch ("value") ) {
        // Get the first long.
        long ms = inbound.nextLong();
        // Get the next int.
        int an0 = inbound.nextInt();
    }

}

void loop() {

inbound.parseStream( &Serial , readMessage );

    // [...]
}
```
Sending (packing) a massage example
-------------------------------------

Add the library to the top of your code and instantiate an AsciiMassagePacker called "outbound":
```cpp
#include <AsciiMassagePacker.h>
AsciiMassagePacker outbound;
```

Pack a *massage* and then steam it through Serial:
```cpp
outbound.beginPacket("value"); // Start a packet with the address called "value".
outbound.addLong( millis() ); // Add the milliseconds.
outbound.addInt( analogRead(0) ); // Add a reading of analog 0.
outbound.streamPacket(&Serial); // End the packet and stream it.
```
