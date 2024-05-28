#pragma once
#ifndef AsyncTelnet_h
#define AsyncTelnet_h

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#else
#error Platform not supported
#endif

typedef std::function<void(void*, AsyncClient*)> ConnHandler;
typedef std::function<void(AsyncClient*)> DisconnHandler;
typedef std::function<void(const char *data, size_t len)> ResponseHandler;

class AsyncTelnet_c {
  public:
    AsyncTelnet_c(uint16_t port = 23);

    bool begin(bool checkConnection = true, bool mDNS = false);
    void stop();

    size_t print(const String &data);
    size_t println(const String &data);    
    size_t write(const char* data, size_t size, uint8_t apiflags=ASYNC_WRITE_FLAG_COPY);          

    bool isClientConnected(AsyncClient *client);
    void disconnectClient();
    uint16_t serverPort() {return(server_port);}
    void onConnect(ConnHandler callbackFunc);
    void onDisconnect(DisconnHandler callbackFunc);
    void onTelnetResponse(ResponseHandler callbackFunc);

    protected:
      AsyncServer server;
      AsyncClient *client;
      boolean isConnected = false;
      IPAddress ip;
      uint16_t server_port = 23;

      ConnHandler on_connect = NULL;
      DisconnHandler on_disconnect = NULL;
      ResponseHandler on_data  = NULL;  
};

#endif

AsyncTelnet_c::AsyncTelnet_c(uint16_t port) : server(port) { 
  server_port = port;
}

bool AsyncTelnet_c::begin(bool checkConnection /* = true */, bool mDNS) {
  if (checkConnection) {
    // connected to WiFi or is ESP in AP mode?
    if (WiFi.status() != WL_CONNECTED) return false;
  }
  server.onClient([=](void *, AsyncClient* c){
    if(c == NULL) return;
    client = c;
    // client->setRxTimeout(10);
    ip = client->remoteIP();
    if(on_connect != NULL) client->onConnect(on_connect, client); 
    c->onDisconnect([=](void *, AsyncClient* c){ 
      if(on_disconnect != NULL) on_disconnect(c); 
      c->close(true);
      c->free();
      delete c;
    }, this);
    if(on_data != NULL){
      c->onData([=](void *, AsyncClient* , void *data, size_t len){ 
        // Serial.println("data incoming");
        on_data((const char *)data, len); 
      }, this);      
    }
    client->setNoDelay(true);
  }, this);
  server.setNoDelay(true);
  server.begin();
  return true;
}

void AsyncTelnet_c::stop() { 
  server.end(); 
}

bool AsyncTelnet_c::isClientConnected(AsyncClient *Client){
  return (Client->connected());
}

void AsyncTelnet_c::disconnectClient() {
  client->stop();
  isConnected = false;
}

size_t AsyncTelnet_c::print(const String &data) {
    if(data == NULL) {
        return 0;
    }
    return client->write(data.c_str(), data.length());
}

size_t AsyncTelnet_c::println(const String &data) {
    if(data == NULL) {
        return 0;
    }
    return client->write((data+"\n").c_str(), data.length()+1);
}

size_t AsyncTelnet_c::write(const char *data, size_t size, uint8_t apiflags) {
    size_t will_send = -client->add(data, size, apiflags);
    if(!will_send || !client->send()) {
        return 0;
    }
    return will_send;
}

void AsyncTelnet_c::onConnect(ConnHandler callbackFunc) { 
  on_connect = callbackFunc; 
}
void AsyncTelnet_c::onDisconnect(DisconnHandler callbackFunc) { 
  on_disconnect = callbackFunc; 
}
void AsyncTelnet_c::onTelnetResponse(ResponseHandler callbackFunc) { 
  on_data = callbackFunc; 
}

/*#include <Arduino.h>
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
*/
