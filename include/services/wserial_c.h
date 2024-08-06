#ifndef __WSERIAL_H
#define __WSERIAL_H
#include <Arduino.h>
#include "ESPTelnet.h"

ESPTelnet Telnet;
class WSerial_c
{
protected:
 uint16_t server_port = 0;
 void (*on_input)(String data) = NULL; 

public:
  bool telnetStart(uint16_t server_port);
  bool telnetStart() { return server_port != 0 ? (telnetStart(server_port)): false; }
  WSerial_c() { Serial.begin(115200);}
  template <typename T>
  void plot(const char *varName, T x, T y, const char *unit = NULL);
  template <typename T>
  void plot(const char *varName, T y, const char *unit = NULL);
  template <typename T>
  void print(const T &data);
  template <typename T>
  void print(const T &data, int base);
  void println();
  template <typename T>
  void println(const T &data);
  template <typename T>
  void println(const T &data, int base);
  size_t printf(const char* format, ...); 
  void update(void);
  uint16_t serverPort() { return (server_port); }
  bool isTelnetOn();
  void telnetStop();  
  void onInputReceived(void (*f)(String data));  
};

bool inline WSerial_c::isTelnetOn()
{
  return Telnet.isConnected();
}

void WSerial_c::telnetStop() {
  Telnet.stop();
}

bool WSerial_c::telnetStart(uint16_t port)
{
  if(isTelnetOn()) Telnet.stop();
  server_port = port;
  Telnet.onDisconnect([](String ip) {
        Serial.print("- Telnet: ");
        Serial.print(ip);
        Serial.println(" disconnected"); 
    }
  );
  Telnet.onConnectionAttempt([](String ip) {
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
  return Telnet.begin(server_port);
}

void WSerial_c::update(void)
{
  Telnet.loop();  //Tem que ser fora do if
  if (!isTelnetOn()) {
    if (Serial.available() && on_input != NULL)
    {
      on_input(Serial.readStringUntil('\n'));
    }
  }
}

template <typename T>
void WSerial_c::plot(const char *varName, T y, const char *unit)
{
  plot(varName, (T)millis(), y, unit);
}
template <typename T>
void WSerial_c::plot(const char *varName, T x, T y, const char *unit)
{
  print(">"); // Inicio de envio de dados para um gráfico.
  print(varName);
  print(":");
  print(x);
  print(":");
  print(y);
  if (unit != NULL)
  {
    print("§"); // Unidade na sequência
    print(unit);
  }
  println("|g"); // Modo Grafico
}


size_t WSerial_c::printf(const char* format, ...){
  va_list arg;
  va_start(arg, format);
  if (isTelnetOn())
    return Telnet.printf(format,arg);  
  else
    return Serial.printf(format,arg);   
}

template <typename T>
void WSerial_c::print(const T &data)
{
  if (isTelnetOn())
    Telnet.print(data);  
  else
    Serial.print(data);  
}

template <typename T>
void WSerial_c::print(const T &data, int base)
{
  if (isTelnetOn())
    Telnet.print(data, base);  
  else
    Serial.print(data, base);  
}

template <typename T>
void WSerial_c::println(const T &data)
{
  if (isTelnetOn())
    Telnet.println(data);  
  else
    Serial.println(data);  
}

template <typename T>
void WSerial_c::println(const T &data, int base)
{
  if (isTelnetOn())
    Telnet.println(data, base);  
else
    Serial.println(data, base); 
}

void WSerial_c::println()
{
  if (isTelnetOn())
    Telnet.println();  
  else
    Serial.println();
}

void WSerial_c::onInputReceived(void (*f)(String data))
{
    on_input = f;
    Telnet.onInputReceived(f);    
}

WSerial_c WSerial;
#endif