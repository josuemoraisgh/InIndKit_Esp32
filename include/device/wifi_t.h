#include <Arduino.h>
#include <WiFi.h>

class Wifi_t
{
private:
    const char *ssid = "APJosue";
    const char *password = "josue32154538";

public:
    bool start(int max_tries = 20, int pause = 500);
    bool isConnected();    
};

bool Wifi_t::start(int max_tries = 20, int pause = 500)
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

bool Wifi_t::isConnected()
{
    return (WiFi.status() == WL_CONNECTED);
}