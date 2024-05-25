#include <Arduino.h>
#include "ESPTelnet.h"

class Telnet_c : public ESPTelnet
{
public:
  uint16_t port = 4000;
  bool start(void);
  void update(void);

};

bool Telnet_c::start()
{
  return(begin(port));
}

void Telnet_c::update()
{
  loop();
}
