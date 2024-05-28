/*
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

    bool begin(bool checkConnection = true);
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

bool AsyncTelnet_c::begin(bool checkConnection) {
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
*/
#include <Arduino.h>
#include "ESPTelnet.h"

class AsyncTelnet_c : public ESPTelnet
{

public:
  bool start(uint16_t server_port = 4000);
  AsyncTelnet_c(uint16_t server_port) : ESPTelnet() { begin(server_port); }
  uint16_t serverPort() { return (this->server_port); }
};

bool AsyncTelnet_c::start(uint16_t server_port)
{
  onDisconnect([](String ip)
               {
        Serial.print("- Telnet: ");
        Serial.print(ip);
        Serial.println(" disconnected"); });
  onConnectionAttempt([](String ip)
                      {
        Serial.print("- Telnet: ");
        Serial.print(ip);
        Serial.println(" tried to connected"); });
  onReconnect([](String ip)
              {
        Serial.print("- Telnet: ");
        Serial.print(ip);
        Serial.println(" reconnected"); });
  return (this->begin(serverPort()));
}
