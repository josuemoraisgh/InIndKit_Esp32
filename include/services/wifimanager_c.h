// link: https://dronebotworkshop.com/wifimanager/

#include <Arduino.h>
#include <WiFiManager.h>

class WifiManager_c : public WiFiManager
{
protected:
    uint8_t trigger_pin;
    uint8_t timeout;
    bool portalRunning = false;
    const char *apName = "OnDemandAP";

public:
    WifiManager_c(const uint8_t &timeout = 120) : WiFiManager() { this->timeout = timeout; }
    void setApName(const char *apName);
    void startAPPortal();
    void changeWebPortal();
};

void WifiManager_c::startAPPortal(/*const char *apName*/)
{
    // reset settings - for testing
    // wm.resetSettings();

    // set configportal timeout
    setConfigPortalTimeout(timeout);

    if (!startConfigPortal(apName))
    {
        Serial.println("failed to connect and hit timeout");
        delay(2000);
        ESP.restart();
        delay(2000);
    }
}

void WifiManager_c::changeWebPortal()
{
      if(!portalRunning){
        Serial.println("Button Pressed, Starting Portal");
        wm.startWebPortal();
        portalRunning = true;
      }
      else{
        Serial.println("Button Pressed, Stopping Portal");
        wm.stopWebPortal();
        portalRunning = false;
      }
}

void WifiManager_c::setApName(const char *apName)
{
    this->apName = apName;
    ((WiFiManager*) this)->setHostname(apName);
}