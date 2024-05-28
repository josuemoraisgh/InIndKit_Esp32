#include <Arduino.h>
#include "ESPTelnet.h"

#define LOG_QUEUE_LEN 50

ESPTelnet Telnet;
static QueueHandle_t logQueue;
static SemaphoreHandle_t LogMutex;

class Telnet_c
{
protected:
  char telnetString[LOG_QUEUE_LEN];

public:
  uint16_t telnetPort = 4000;
  bool telnetStart(void);
  void telnetLoop();
  void telnetSendQueue(const char *stringAux);
};

bool Telnet_c::telnetStart()
{
  LogMutex = xSemaphoreCreateBinary();
  logQueue = xQueueCreate(LOG_QUEUE_LEN, sizeof(char));

  Telnet.onConnect([](String ip)
                   {
      Serial.print("- Telnet: ");
      Serial.print(ip);
      Serial.println(" connected");

      Telnet.println("\nWelcome " + ip);
      Telnet.println("(Use ^] + q  to disconnect.)"); });
  Telnet.onDisconnect([](String ip)
                      {
      Serial.print("- Telnet: ");
      Serial.print(ip);
      Serial.println(" disconnected"); });
  Telnet.onConnectionAttempt([](String ip)
                             {
      Serial.print("- Telnet: ");
      Serial.print(ip);
      Serial.println(" tried to connected"); });
  Telnet.onReconnect([](String ip)
                     {
      Serial.print("- Telnet: ");
      Serial.print(ip);
      Serial.println(" reconnected"); });
  Telnet.onInputReceived([](String str)
                         {
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
      } });
  return (Telnet.begin(telnetPort));
}

void Telnet_c::telnetLoop()
{
  xQueueReceive(logQueue, (void *)&telnetString, 0);
  //if (Telnet.isConnected())
    Telnet.print(telnetString);
  //else
    //Serial.print(telnetString);
  Telnet.loop();
}


void Telnet_c::telnetSendQueue(const char *stringAux)
{
  if (xSemaphoreTake(LogMutex, (TickType_t) 10) == pdTRUE)
  {
    xQueueSend(logQueue, (void *)&stringAux, portMAX_DELAY);
    xSemaphoreGive(LogMutex);
  }
}
