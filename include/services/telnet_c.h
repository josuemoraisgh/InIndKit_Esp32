#include <Arduino.h>
#include "ESPTelnet.h"

ESPTelnet Telnet;

class Telnet_c
{
public:
  uint16_t telnetPort = 4000;
  bool telnetStart(void);
};

bool Telnet_c::telnetStart()
{
  Telnet.onConnect([](String ip) {
      Serial.print("- Telnet: ");
      Serial.print(ip);
      Serial.println(" connected");

      Telnet.println("\nWelcome " + ip);
      Telnet.println("(Use ^] + q  to disconnect.)"); 
    }
  );
  Telnet.onDisconnect([](String ip) {
      Serial.print("- Telnet: ");
      Serial.print(ip);
      Serial.println(" disconnected"); 
    }
  );
  Telnet.onConnectionAttempt([](String ip){
      Serial.print("- Telnet: ");
      Serial.print(ip);
      Serial.println(" tried to connected");
    }
  );
  Telnet.onReconnect([](String ip) {
      Serial.print("- Telnet: ");
      Serial.print(ip);
      Serial.println(" reconnected");
    }
  );
  Telnet.onInputReceived([](String str) {
      // checks for a certain command
      if (str == "ping") {
        Telnet.println("> pong"); 
        Serial.println("- Telnet: pong");
      // disconnect the client
      } else if (str == "bye") {
        Telnet.println("> disconnecting you...");
        Telnet.disconnectClient();
      } else {
        Telnet.println(str);
      } 
    }
  );
  return (Telnet.begin(telnetPort));
}