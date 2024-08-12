// link: https://dronebotworkshop.com/wifimanager/

#include <Arduino.h>
#include <WiFiManager.h>
#include "services/WSerial_c.h"
class WifiManager_c : public WiFiManager
{
protected:
    WSerial_c* WSerial;
    uint8_t trigger_pin;
    uint8_t timeout;
    bool portalRunning = false;
    const char *apName = "OnDemandAP";


public:
    WifiManager_c(const uint8_t &timeout = 120) : WiFiManager() {this->timeout = timeout;}
    void start(WSerial_c* ws) {WSerial = ws;}        
    void setApName(const char *apName);
    bool changeWebPortal();
    bool getPortalRunning(){return portalRunning;}
    void resetSettingsRestart();
};



void WifiManager_c::resetSettingsRestart()
{
    resetSettings();
    WSerial->println("Rebooting now...");
    delay(2000);
    ESP.restart();
    delay(2000);
}

bool WifiManager_c::changeWebPortal()
{
    if (!portalRunning)
    {
        WSerial->println("Button Pressed, Starting Portal");
        setConfigPortalBlocking(false);
        startWebPortal();
        portalRunning = true;
        return true;
    }
    else
    {
        WSerial->println("Button Pressed, Stopping Portal");
        stopWebPortal();
        portalRunning = false;
        return false;
    }
}

void WifiManager_c::setApName(const char *apName)
{
    this->apName = apName;
    ((WiFiManager *)this)->setHostname(apName);
}