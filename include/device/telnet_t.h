#include <Arduino.h>
#include "ESPTelnet.h"

ESPTelnet telnet;

class Telnet_t
{
private:
  IPAddress ip;
  uint16_t port = 4000;

public:
  bool start(void);
  void update(void);
};

void Telnet_t::update()
{
  telnet.loop();
}

bool Telnet_t::start()
{
  Serial.print("- Telnet: Is runing");
  // passing on functions for various telnet events
  telnet.onConnect(onTelnetConnect);
  telnet.onConnectionAttempt(onTelnetConnectionAttempt);
  telnet.onReconnect(onTelnetReconnect);
  telnet.onDisconnect(onTelnetDisconnect);
  telnet.onInputReceived(onTelnetInput);

  Serial.print("- Telnet: ");
  return (telnet.begin(port));
}

/* ------------------------------------------------- */

// (optional) callback functions for telnet events
void onTelnetConnect(String ip)
{
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" connected");

  telnet.println("\nWelcome " + telnet.getIP());
  telnet.println("(Use ^] + q  to disconnect.)");
}

void onTelnetDisconnect(String ip)
{
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" disconnected");
}

void onTelnetReconnect(String ip)
{
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" reconnected");
}

void onTelnetConnectionAttempt(String ip)
{
  Serial.print("- Telnet: ");
  Serial.print(ip);
  Serial.println(" tried to connected");
}

void onTelnetInput(String str)
{
  // checks for a certain command
  if (str == "ping")
  {
    telnet.println("> pong");
    Serial.println("- Telnet: pong");
    // disconnect the client
  }
  else if (str == "bye")
  {
    telnet.println("> disconnecting you...");
    telnet.disconnectClient();
  }
  else
  {
    telnet.println(str);
  }
}
