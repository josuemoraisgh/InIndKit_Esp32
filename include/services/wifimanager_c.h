//link: https://dronebotworkshop.com/wifimanager/

#include <Arduino.h>
#include <WiFiManager.h>

class WifiManager_c : public WiFiManager
{
protected:
    uint8_t trigger_pin;
    uint8_t timeout;

public:
    WifiManager_c(const uint8_t &timeout = 120) : WiFiManager() { this->timeout = timeout; }
    void onStart();
};

void WifiManager_c::onStart()
{
    // reset settings - for testing
    // wm.resetSettings();

    // set configportal timeout
    setConfigPortalTimeout(timeout);

    if (!startConfigPortal("OnDemandAP"))
    {
        Serial.println("failed to connect and hit timeout");
        delay(2000);
        // reset and try again, or maybe put it to deep sleep
        ESP.restart();
        delay(2000);
    }
}