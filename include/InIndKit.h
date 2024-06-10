#ifndef __ININDKIT_H
#define __ININDKIT_H

#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>

#include "services\display_c.h"
#include "services\ota_c.h"
#include "services\wifi_c.h"
#include "services\telnet_c.h"
#include "services\hart_c.h"
#include "util/asyncDelay.h"
#include "util/btn.h"
//////////////////////////Lado Esquerdo///////////////////////
/********** POTENTIOMETERS GPIO define *****/
#define def_pin_POT_LEFT 36  // GPIO36
#define def_pin_POT_RIGHT 39 // GPIO39
/******************** Inputs **************/
#define def_pin_IN1 33 // GPIO33
#define def_pin_IN2 32 // GPIO32
#define def_pin_IN3 35 // GPIO35
#define def_pin_IN4 34 // GPIO34
/********************* DAC ****************/
#define def_pin_DAC1 25 // GPIO25
#define def_pin_DAC2 26 // GPIO26
/******************** Outputs **************/
#define def_pin_OUT1 27  // GPIO27
#define def_pin_OUT2 14  // GPIO14
#define def_pin_OUT3 12  // GPIO12
#define def_pin_OUT4 13  // GPIO13
/**************** Hart Interface **********/
#define def_pin_Hart_RXD 9  // Pino RX da ESP32 conectado ao pino RX do DS8500
#define def_pin_Hart_TXD 10 // Pino TX da ESP32 conectado ao pino TX do DS8500
#define def_pin_Hart_RTS 11 // Pino RTS da ESP32 conectado ao pino RTS do DS8500
#define def_pin_Hart_CTS 6  // Pino CD da ESP32 conectado ao pino CD do DS8500
//////////////////////////Lado Direito///////////////////////
/********************* RELÊ ***************/
#define def_pin_RELE 23 // GPIO23
/***************** OLED Display ************/
#define def_pin_SDA 21 // GPIO21
#define def_pin_SCL 22 // GPIO22
/********************* PWM ****************/
#define def_pin_PWM 1 // GPIO1
/***************** Write 4@20 mA **********/
#define def_pin_W4a20_1 3 // GPIO3
/************* BUTTONS GPIO define *********/
#define def_pin_PUSH1 18 // GPIO18
#define def_pin_PUSH2 19 // GPIO19
#define def_pin_RTN1 17  // GPIO17
#define def_pin_RTN2 5   // GPIO5
/***************** Read 4@20 mA ***********/
#define def_pin_R4a20_1 4 // GPIO4
#define def_pin_R4a20_2 0 // GPIO0


#define HOSTNAME "inindkit0"

Telnet_c Telnet(4000);
Hart_c ds8500Serial(4001);

// Use ESP, InIndKit, WiFi, ArduinoOTA, InIndKit.Display e InIndKit.Telnet
class InIndKit_c : public Wifi_c, public OTA_c, public Display_c
{
public:
    btn_t rtn_1 = {def_pin_RTN1, 0, false, false};
    btn_t rtn_2 = {def_pin_RTN2, 0, false, false};
    btn_t push_1 = {def_pin_PUSH1, 0, false, false};
    btn_t push_2 = {def_pin_PUSH2, 0, false, false};

    void setup(const char *ssid, const char *password);
    void loop(void);
    void errorMsg(String error, bool restart = true);
};
#endif

inline void InIndKit_c::setup(const char *ssid, const char *password)
{
    Serial.begin(115200);
    Serial.println("Booting");

    /********** POTENTIOMETERS GPIO define *****/
    pinMode(def_pin_POT_LEFT, INPUT);
    pinMode(def_pin_POT_RIGHT, INPUT);
    /************* BUTTONS GPIO define *********/
    pinMode(def_pin_RTN1, INPUT);
    pinMode(def_pin_RTN2, INPUT);
    pinMode(def_pin_PUSH1, INPUT);
    pinMode(def_pin_PUSH2, INPUT);
    /*************** IOs GPIO define **********/
    pinMode(def_pin_IN1, INPUT);
    pinMode(def_pin_IN2, INPUT);
    pinMode(def_pin_IN3, INPUT);
    pinMode(def_pin_IN4, INPUT);    
    pinMode(def_pin_OUT1, OUTPUT);
    pinMode(def_pin_OUT2, OUTPUT);
    pinMode(def_pin_OUT3, OUTPUT);
    pinMode(def_pin_OUT4, OUTPUT);    
    /********************* PWM ****************/
    pinMode(def_pin_PWM, OUTPUT);
    /********************* DAC ****************/
    pinMode(def_pin_DAC1, OUTPUT);
    pinMode(def_pin_DAC2, OUTPUT);    
    /********************* RELÊ ***************/
    pinMode(def_pin_RELE, OUTPUT);
    /***************** Read 4@20 mA ***********/
    pinMode(def_pin_R4a20_1, INPUT);
    pinMode(def_pin_R4a20_2, INPUT);
    /***************** Write 4@20 mA **********/
    pinMode(def_pin_W4a20_1, OUTPUT);

    if (displayStart())
    {
        Serial.println("Display running");
        setDisplayText(1, "WiFi");
        setDisplayText(2, "connecting");
        setDisplayText(3, "  Wait!!");
        displayUpdate();
        delay(2500);
    }
    else
    {
        errorMsg("Display error.", false);
    }
    if (wifiStart(ssid, password)) // Primeiro o Wifi
    {
        Serial.print("\nWifi running - IP:");
        Serial.println(WiFi.localIP());
        setDisplayText(1, WiFi.localIP().toString().c_str());
        setDisplayText(2, "InIndKit01 ");
        setDisplayText(3, "UFU Mode");
        displayUpdate();
        delay(2500);
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

    if (Telnet.start())
    {
        Serial.print("Telnet running - port:");
        Serial.println(Telnet.serverPort());
    }
    else
    {
        errorMsg("Telnet  error.\nWill reboot...");
    }

    Telnet.onConnect([](String ip)
                     {
        Telnet.println("\nWelcome " + ip);
        Telnet.println("(Use ^] + q  to disconnect.)"); });
    
    ds8500Serial.setup();
}

void InIndKit_c::loop(void)
{
    ArduinoOTA.handle();
    Telnet.update();
    displayUpdate();
    ds8500Serial.loop();
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