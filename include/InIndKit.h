#ifndef __ININDKIT_H
#define __ININDKIT_H

#include <Arduino.h>

#include <WiFi.h>
#include "services\display_c.h"
#include "services\ota_c.h"
#include "services\wifi_c.h"
#include "services\telnet_c.h"
#include "util/btn.h"

/***************** OLED Display ************/
#define def_pin_SDA 21 // GPIO21
#define def_pin_SCL 22 // GPIO22
/********** POTENTIOMETERS GPIO define *****/
#define def_pin_POT_LEFT 36  // GPIO36
#define def_pin_POT_RIGHT 39 // GPIO39
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

// Use ESP, InIndKit, WiFi, ArduinoOTA, InIndKit.Display e InIndKit.Telnet
class InIndKit_c : public Wifi_c, OTA_c, Telnet_c
{
public:
    btn_t rtn_1 = {def_pin_RTN1, 0, false, false};
    btn_t rtn_2 = {def_pin_RTN2, 0, false, false};
    btn_t push_1 = {def_pin_PUSH1, 0, false, false};
    btn_t push_2 = {def_pin_PUSH2, 0, false, false};

    Display_c Display;

    void start(void);
    void update(void);
    void errorMsg(String error, bool restart = true);

    template <typename T>
    void println(const T &data);
    template <typename T>
    void println(const T &data, int base);
    void println();

    template <typename T>
    void print(const T &data);
    template <typename T>
    void print(const T &data, int base);
};

inline void InIndKit_c::start(void)
{
    Serial.begin(115200);
    Serial.println("Booting");
    if (wifiStart()) // Primeiro o Wifi
    {
        Serial.print("\nWifi running - IP:");
        Serial.print(WiFi.localIP());
        Serial.println(".");
    }
    else
    {
        errorMsg("Wifi  error.\nWill reboot...");
    }

    otaStart(HOSTNAME); // Depois o OTA

    pinMode(def_pin_POT_LEFT, INPUT);
    pinMode(def_pin_POT_RIGHT, INPUT);

    pinMode(def_pin_RTN1, INPUT);
    pinMode(def_pin_RTN2, INPUT);
    pinMode(def_pin_PUSH1, INPUT);
    pinMode(def_pin_PUSH2, INPUT);

    pinMode(def_pin_IN1, INPUT);
    pinMode(def_pin_IN2, INPUT);

    if (Display.start())
    {
        Serial.println("Display running");
        Display.setText(1, WiFi.localIP().toString().c_str());
        Display.setText(2, "InIndKit01 ");
        Display.setText(3, "Good Look!");
    }
    else
    {
        errorMsg("Display error.", false);
    }

    if (telnetStart())
    {
        Serial.print("Telnet running - port:");
        Serial.print(telnetPort);
        Serial.println(".");
    }
    else
    {
        errorMsg("Telnet  error.\nWill reboot...");
    }
}

void InIndKit_c::update(void)
{
    ArduinoOTA.handle();
    telnetLoop();
    Display.update();
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

template <typename T>
void InIndKit_c::println(const T &data)
{
    telnetSendQueue((String(data) + "\n").c_str());
}

template <typename T>
void InIndKit_c::println(const T &data, int base)
{
    telnetSendQueue((String(data, base) + "\n").c_str());
}

void InIndKit_c::println()
{
    telnetSendQueue(String("\n").c_str());
}

template <typename T>
void InIndKit_c::print(const T &data)
{
    telnetSendQueue(String(data).c_str());
}

template <typename T>
void InIndKit_c::print(const T &data, int base)
{
    telnetSendQueue(String(data, base).c_str());
}

InIndKit_c InIndKit;
#endif