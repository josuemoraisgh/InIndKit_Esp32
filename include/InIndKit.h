#ifndef __ININDKIT_H
#define __ININDKIT_H

#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>

#include "services\display_c.h"
#include "services\ota_c.h"
#include "services\wifi_c.h"
#include "services\asyncTelnet_c.h"
#include "util/asyncDelay.h"
#include "util/btn.h"

/***************** OLED Display ************/
#define def_pin_SDA 21 // GPIO21
#define def_pin_SCL 22 // GPIO22
/********** POTENTIOMETERS GPIO define *****/
#define def_pin_POT_LEFT 39  // GPIO36
#define def_pin_POT_RIGHT 36 // GPIO39
/************* BUTTONS GPIO define *********/
#define def_pin_RTN1 17  // GPIO17
#define def_pin_RTN2 5   // GPIO5
#define def_pin_PUSH1 18 // GPIO18
#define def_pin_PUSH2 19 // GPIO19
/*************** IOs GPIO define **********/
#define def_pin_IN1 14  // GPIO14
#define def_pin_IN2 12  // GPIO12
#define def_pin_OUT1 35 // GPIO3
#define def_pin_OUT2 35 // GPIO3

#define HOSTNAME "inindkit0"
#define TELNETPORT 4000

AsyncTelnet_c Telnet(TELNETPORT);

// Use ESP, InIndKit, WiFi, ArduinoOTA, InIndKit.Display e InIndKit.Telnet
class InIndKit_c : public Wifi_c, public OTA_c, public Display_c
{
protected:
    const char *ssid = "APJosue";
    const char *password = "josue32154538";

public:
    btn_t rtn_1 = {def_pin_RTN1, 0, false, false};
    btn_t rtn_2 = {def_pin_RTN2, 0, false, false};
    btn_t push_1 = {def_pin_PUSH1, 0, false, false};
    btn_t push_2 = {def_pin_PUSH2, 0, false, false};

    void start(void);
    void loop(void);
    void errorMsg(String error, bool restart = true);
};
#endif

inline void InIndKit_c::start(void)
{
    Serial.begin(115200);
    Serial.println("Booting");
    if (displayStart())
    {
        Serial.println("Display running");
        setDisplayText(1, "Wifi Starting");
        setDisplayText(2, "InIndKit01 ");
        setDisplayText(3, "Good Look!");
    }
    else
    {
        errorMsg("Display error.", false);
    }
    if (wifiStart(ssid, password)) // Primeiro o Wifi
    {
        Serial.print("\nWifi running - IP:");
        Serial.print(WiFi.localIP());
        Serial.println(".");
        setDisplayText(1, WiFi.localIP().toString().c_str());
        setDisplayText(2, "InIndKit01 ");
        setDisplayText(3, "Good Look!");        
    }
    else
    {
        errorMsg("Wifi  error.\nWill reboot...");
    }
    // if (!MDNS.begin(HOSTNAME))
    //{
    //     errorMsg("MDNS Error.\nWill reboot...");
    // }
    // MDNS.addService("http", "tcp", 80);

    otaStart(HOSTNAME); // Depois o OTA

    pinMode(def_pin_POT_LEFT, INPUT);
    pinMode(def_pin_POT_RIGHT, INPUT);

    pinMode(def_pin_RTN1, INPUT);
    pinMode(def_pin_RTN2, INPUT);
    pinMode(def_pin_PUSH1, INPUT);
    pinMode(def_pin_PUSH2, INPUT);

    pinMode(def_pin_IN1, INPUT);
    pinMode(def_pin_IN2, INPUT);

    if (Telnet.start())
    {
        Serial.print("Telnet running - port:");
        Serial.print(Telnet.serverPort());
        Serial.println(".");
    }
    else
    {
        errorMsg("Telnet  error.\nWill reboot...");
    }
}

void InIndKit_c::loop(void)
{
    ArduinoOTA.handle();
    Telnet.loop();
    displayUpdate();
}

void InIndKit_c::errorMsg(String error, bool restart)
{
    Serial.println(error);
    if (restart)
    {
        Serial.println("Rebooting now...");
        delay(2000);
        ESP.restart();
        delay(2000);
    }
}
InIndKit_c InIndKit;