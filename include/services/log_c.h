#include <Arduino.h>
#include "ESPTelnet.h"

ESPTelnet Telnet;
static QueueHandle_t logQueue;
static int log_queue_len = 10;
static SemaphoreHandle_t LogMutex;

class Log_c
{
protected:
  String logString;

public:
  uint16_t logPort = 4000;
  bool logStart(void);
  void logLoop();
  void sendQueue(const String stringAux);

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
  xQueueReceive(logQueue, (void *)&logString, portMAX_DELAY);
  if (Telnet.isConnected())
    Telnet.print(logString);
  else
    Serial.print(logString);
  Telnet.loop();
}

void Log_c::sendQueue(const String stringAux)
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
  sendQueue(String(data)+"\n");
}

template <typename T>
inline void Log_c::logPrintln(const T &data, int base)
{
  sendQueue(String(data,base)+"\n");
}

inline void Log_c::logPrintln()
{
  sendQueue(String("\n"));
}

template <typename T>
inline void Log_c::logPrint(const T &data)
{
  sendQueue(String(data));
}

template <typename T>
inline void Log_c::logPrint(const T &data, int base)
{
  sendQueue(String(data,base));
}
