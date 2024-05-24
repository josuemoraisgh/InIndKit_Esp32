#ifndef __ININDKIT_H
#define __ININDKIT_H

#include <Arduino.h>

#include <WiFi.h>
#include "device\display_t.h"
#include "device\ota_t.h"
#include "device\wifi_t.h"
#include "device\telnet_t.h"

/***************** OLED Display ************/
#define def_pin_SDA 21 // GPIO21
#define def_pin_SCL 22 // GPIO22
/********** POTENTIOMETERS GPIO define *****/
#define def_pin_POT_LEFT 36  // GPIO36
#define def_pin_POT_RIGHT 39 // GPIO39
/************* BUTTONS GPIO define *********/
#define def_pin_RTN1 34  // GPIO34
#define def_pin_RTN2 35  // GPIO35
#define def_pin_PUSH1 19 // GPIO19
#define def_pin_PUSH2 18 // GPIO18
/*************** IOs GPIO define **********/
#define def_pin_IN1 14  // GPIO14
#define def_pin_IN2 12  // GPIO12
#define def_pin_OUT1 35 // GPIO3
#define def_pin_OUT2 35 // GPIO3

class InIndKit
{
private:
    const char *ssid = "APJosue";
    const char *password = "josue32154538";

    Wifi_t wifi_v;
    OTA_t ota_v;
    Display_t display_v;
    Telnet_t telnet_v;

public:
    InIndKit(void);
    void update(void);
    void errorMsg(String error, bool restart = true);
};

InIndKit::InIndKit()
{
    Serial.begin(115200);
    Serial.println("Booting");

    pinMode(def_pin_RTN1, INPUT);
    pinMode(def_pin_RTN2, INPUT);
    pinMode(def_pin_PUSH1, INPUT);
    pinMode(def_pin_PUSH2, INPUT);

    pinMode(def_pin_IN1, INPUT);
    pinMode(def_pin_IN2, INPUT);

    if (wifi_v.start()) // Primeiro o Wifi
        Serial.println("Wifi running");
    else
        errorMsg("Wifi  error.\nWill reboot...");

    ota_v.start(); // Depois o OTA

    if (display_v.start())    
        Serial.println("Display running");
    else
        errorMsg("Display error.",false);

    if (telnet_v.start())
        Serial.println("Telnet running");
    else
        errorMsg("Telnet  error.\nWill reboot...");
}

void InIndKit::update(void)
{
    ota_v.update();
    display_v.update();
}

void InIndKit::errorMsg(String error, bool restart = true)
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

#endif