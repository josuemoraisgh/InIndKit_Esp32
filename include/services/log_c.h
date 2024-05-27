#include <Arduino.h>
#include "ESPTelnet.h"

#define LOG_QUEUE_LEN 50;

ESPTelnet Telnet;
static QueueHandle_t logQueue;
static SemaphoreHandle_t LogMutex;

class Log_c
{
protected:
  char logString[LOG_QUEUE_LEN];

public:
  uint16_t logPort = 4000;
  bool logStart(void);
  void logLoop();
  void sendQueue(const char *stringAux);

  template <typename T>
  void logPrintln(const T &data);
  template <typename T>
  void logPrintln(const T &data, int base);
  void logPrintln();

  template <typename T>
  void logPrint(const T &data);
  template <typename T>
  void logPrint(const T &data, int base);
};

bool Log_c::logStart()
{
  LogMutex = xSemaphoreCreateBinary();

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
  return (Telnet.begin(logPort));
}

void Log_c::logLoop()
{
  xQueueReceive(logQueue, (void *)&logString, 0);
  if (Telnet.isConnected())
    Telnet.print(logString);
  else
    Serial.print(logString);
  Telnet.loop();
}

void Log_c::sendQueue(const char *stringAux)
{
  if (xSemaphoreTake(LogMutex, (TickType_t)10) == pdTRUE)
  {
    xQueueSend(logQueue, (void *)&stringAux, portMAX_DELAY);
    xSemaphoreGive(LogMutex);
  }
}

template <typename T>
inline void Log_c::logPrintln(const T &data)
{
  sendQueue((String(data)+"\n").c_str());
}

template <typename T>
inline void Log_c::logPrintln(const T &data, int base)
{
  sendQueue((String(data,base)+"\n").c_str());
}

inline void Log_c::logPrintln()
{
  sendQueue(String("\n").c_str());
}

template <typename T>
inline void Log_c::logPrint(const T &data)
{
  sendQueue(String(data).c_str());
}

template <typename T>
inline void Log_c::logPrint(const T &data, int base)
{
  sendQueue(String(data,base).c_str());
}
