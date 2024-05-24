#include <Arduino.h>
#include <WiFi.h>

class Wifi_c
{
private:
    const char *ssid = "APJosue";
    const char *password = "josue32154538";

public:
    bool start(uint16_t max_tries = 20, uint16_t pause = 500);
    IPAddress getIP();
    bool isConnected();
};

bool Wifi_c::start(uint16_t max_tries, uint16_t pause)
{
    int i = 0;
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

#if defined(ARDUINO_ARCH_ESP8266)
    WiFi.forceSleepWake();
    delay(200);
#endif
    WiFi.begin(ssid, password);
    do
    {
        delay(pause);
        Serial.print(".");
        i++;
    } while (!isConnected() && i < max_tries);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    return isConnected();
}

IPAddress Wifi_c::getIP()
{
    return(WiFi.localIP());
}

bool Wifi_c::isConnected()
{
    return (WiFi.status() == WL_CONNECTED);
}