#include <AsciiMassenger.h>


class AsciiMassengerUdp : public AsciiMassenger {
    UDP* udp;
    
    public:
      AsciiMassengerUdp(UDP* udp) : AsciiMassenger(udp) {
        this->udp = udp;
        flush();
      }

      bool parsePacket() {
        int packetSize = udp->parsePacket();
        
          if (packetSize) {
            return AsciiMassenger::parsePacket();
          }

          return false;
          
      }

      void beginPacket(const char* addr, IPAddress remoteIp, int remotePort) {
        udp->beginPacket(remoteIp, remotePort );
        AsciiMassenger::beginPacket(addr);
      }

      void endPacket() {
          AsciiMassenger::endPacket();
          udp->endPacket();
      }
  
};

