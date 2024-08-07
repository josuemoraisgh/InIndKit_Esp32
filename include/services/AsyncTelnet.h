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
#include <ESPmDNS.h>

typedef std::function<void(void *, AsyncClient *)> ConnHandler;
typedef std::function<void(AsyncClient *)> DisconnHandler;
typedef std::function<void(const char *data, size_t len)> ResponseHandler;

class AsyncTelnet
{
public:
    AsyncTelnet(uint16_t port = 23);

    bool begin(bool checkConnection = true, bool mDNS = false);
    void stop();

    size_t write(const char *data);
    size_t write(const char *data, size_t size, uint8_t apiflags = ASYNC_WRITE_FLAG_COPY);

    bool isClientConnected(AsyncClient *client);
    void disconnectClient();

    IPAddress getIP() const;
    IPAddress getLastAttemptIP() const;

    void onConnect(ConnHandler callbackFunc);
    void onDisconnect(DisconnHandler callbackFunc);
    void onTelnetResponse(ResponseHandler callbackFunc);

    // protected:
    AsyncServer server;
    AsyncClient *client;
    boolean isConnected = false;
    IPAddress ip;
    uint16_t server_port = 23;

    ConnHandler on_connect = NULL;
    DisconnHandler on_disconnect = NULL;
    ResponseHandler on_data = NULL;
};

#endif

AsyncTelnet::AsyncTelnet(uint16_t port) : server(port)
{
    server_port = port;
}

bool AsyncTelnet::begin(bool checkConnection /* = true */, bool mDNS)
{
    if (checkConnection)
    {
        // connected to WiFi or is ESP in AP mode?
        if (WiFi.status() != WL_CONNECTED)
            return false;
        if (mDNS)
        {
            if (!MDNS.begin("esp32"))
            {
                Serial.println("Error setting up MDNS responder!");
                while (1)
                {
                    delay(1000);
                }
            }
            MDNS.addService("http", "tcp", 80);
        }
    }
    server.onClient([=](void *, AsyncClient *c)
                    {
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
    client->setNoDelay(true); }, this);
    server.setNoDelay(true);
    server.begin();
    return true;
}

void AsyncTelnet::stop()
{
    server.end();
}

bool AsyncTelnet::isClientConnected(AsyncClient *Client)
{
    return (Client->connected());
}

void AsyncTelnet::disconnectClient()
{
    client->stop();
    isConnected = false;
}

size_t AsyncTelnet::write(const char *data)
{
    if (data == NULL)
    {
        return 0;
    }
    return client->write(data, strlen(data));
}

size_t AsyncTelnet::write(const char *data, size_t size, uint8_t apiflags)
{
    size_t will_send = -client->add(data, size, apiflags);
    if (!will_send || !client->send())
    {
        return 0;
    }
    return will_send;
}

void AsyncTelnet::onConnect(ConnHandler callbackFunc)
{
    on_connect = callbackFunc;
}
void AsyncTelnet::onDisconnect(DisconnHandler callbackFunc)
{
    on_disconnect = callbackFunc;
}
void AsyncTelnet::onTelnetResponse(ResponseHandler callbackFunc)
{
    on_data = callbackFunc;
}