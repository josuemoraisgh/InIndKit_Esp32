#include <Arduino.h>
#include <WiFi.h>

class Wifi_c
{

public:
    bool wifiStart(const char *ssid, const char *password, uint16_t max_tries = 20, uint16_t pause = 500);
    bool wifiIsConnected();
};

inline bool Wifi_c::wifiStart(const char *ssid, const char *password, uint16_t max_tries, uint16_t pause)
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
    } while (!wifiIsConnected() && i < max_tries);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    return wifiIsConnected();
}

inline bool Wifi_c::wifiIsConnected()
{
    return (WiFi.status() == WL_CONNECTED);
}