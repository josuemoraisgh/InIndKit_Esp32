#include <Arduino.h>
#include "ESPTelnet.h"

class Telnet_c
{
public:
  ESPTelnet telnet;
  uint16_t port = 4000;
  bool start(void);
  void update(void);
};

bool Telnet_c::start()
{
  return(telnet.begin(port));
}

void Telnet_c::update()
{
  telnet.loop();
}
