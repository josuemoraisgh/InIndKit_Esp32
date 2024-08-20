#ifndef __WSERIAL_H
#define __WSERIAL_H
#include <Arduino.h>
#include <AsyncTelnet.h>

#define BAUD_RATE 115200
#define NEWLINE "\r\n"

class WSerial_c
{
protected:
  uint16_t server_port = 0;
  AsyncTelnet *_telnet;
  bool isClientConnected;
  std::function<void(std::string)> on_input;
  void start(uint16_t port, unsigned long baudrate = BAUD_RATE);
  void update();  

public:
  uint16_t serverPort() { return (server_port); }
  void stop();
  friend inline void startWSerial(WSerial_c *ws,uint16_t port, unsigned long baudrate = BAUD_RATE);
  friend inline void updateWSerial(WSerial_c *ws);   

  template <typename T>
  void print(const T &data);

  template <typename T>
  void println(const T &data);
  void println();

  template <typename T>
  void plot(const char *varName, TickType_t x, T y, const char *unit = NULL);
  template <typename T>
  void plot(const char *varName, T y, const char *unit = NULL);

  void onInputReceived(std::function<void(std::string)> callback);
};

void WSerial_c::stop()
{
  _telnet->stop();
}

inline void startWSerial(WSerial_c *ws,uint16_t port, unsigned long baudrate){ws->start(port, baudrate);}
void WSerial_c::start(uint16_t port, unsigned long baudrate)
{
  if (isClientConnected)
  {
    _telnet->stop();
    delete (_telnet);
  }
  isClientConnected = false;
  server_port = port;
  Serial.begin(baudrate);
  _telnet = new AsyncTelnet(server_port);
  Serial.println();
  _telnet->onConnect([=](void *, AsyncClient *client)
                     {
            Serial.println("\nClient connected");
            isClientConnected = true; });

  _telnet->onDisconnect([=](AsyncClient *client)
                        {
            Serial.println("\nClient disconnected");
            isClientConnected = false; });

  _telnet->onIncomingData([=](const std::string &data)
                          { print(data.c_str()); });
  _telnet->begin(false, false);
  println();
}

inline void updateWSerial(WSerial_c *ws) {ws->update();}
void WSerial_c::update() {
  if(!isClientConnected) {
    if(Serial.available()) {
      on_input(std::string((Serial.readStringUntil('\n')).c_str()));
    }
  }
}

template <typename T>
void WSerial_c::plot(const char *varName, T y, const char *unit)
{
  plot(varName,(TickType_t) xTaskGetTickCount(), y, unit);
}
template <typename T>
void WSerial_c::plot(const char *varName, TickType_t x, T y, const char *unit)
{
  String str(">");
  str.concat(varName);
  str.concat(":");
  str.concat(x);
  str.concat(":");
  str.concat(y);
  str.concat(unit != NULL ? "§" : "");
  str.concat(unit != NULL ? unit : "");  
  str.concat("|g");
  println(str);
}

template <typename T>
void WSerial_c::print(const T &data)
{
  if (isClientConnected)
    _telnet->write(String(data).c_str());
  else
    Serial.print(data);
}

template <typename T>
void WSerial_c::println(const T &data)
{
  String str(data);
  str.concat(NEWLINE);
  print(str);
}
void WSerial_c::println()
{
  if (isClientConnected)
  {
    _telnet->write(NEWLINE);
  }
  else
    Serial.println();
}

void WSerial_c::onInputReceived(std::function<void(std::string)> callback)
{
  _telnet->onIncomingData(callback);
  on_input = callback;
}

#endif