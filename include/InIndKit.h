#ifndef __ININDKIT_H
#define __ININDKIT_H

#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>

#include "services\display_c.h"
#include "services\ota_c.h"
#include "services\wifi_c.h"
#include "services\UDP_c.h"
#include "services\telnet_c.h"
#include "services\hart_c.h"
#include "services\wifimanager_c.h"
#include "util/asyncDelay.h"
#include "util/btn.h"

//////////////////////////Lado Esquerdo///////////////////////
/********** POTENTIOMETERS GPIO define *****/
#define def_pin_POT_LEFT 36  // GPIO36
#define def_pin_POT_RIGHT 39 // GPIO39
/******************** Inputs **************/
#define def_pin_IN1 27 // GPIO27 - Não funciona como entrada analogica somente digital
#define def_pin_IN2 34 // GPIO34
#define def_pin_IN3 14 // GPIO14 - Não funciona como entrada analogica somente digital
#define def_pin_IN4 35 // GPIO35
/********************* DAC ****************/
#define def_pin_DAC1 25 // GPIO25
#define def_pin_DAC2 26 // GPIO26
/***************** Read 4@20 mA ***********/
#define def_pin_R4a20_1 33 // GPIO33
#define def_pin_R4a20_2 32 // GPIO32
/******************** Outputs **************/
#define def_pin_OUT1 12 // GPIO12
#define def_pin_OUT2 13 // GPIO13
#define def_pin_OUT3 2  // GPIO2
#define def_pin_OUT4 15 // GPIO15
/**************** Hart Interface **********/
#define def_pin_Hart_RXD 3 // Pino RX da ESP32 conectado ao pino RX do DS8500
#define def_pin_Hart_TXD 1 // Pino TX da ESP32 conectado ao pino TX do DS8500
#define def_pin_Hart_RTS 22  // Pino RTS da ESP32 conectado ao pino RTS do DS8500
#define def_pin_Hart_CTS 19  // Pino CD da ESP32 conectado ao pino CD do DS8500
//////////////////////////Lado Direito///////////////////////
/********************* RELÊ ***************/
#define def_pin_RELE 23 // GPIO23
/***************** OLED Display ************/
#define def_pin_SDA 21 // GPIO21
#define def_pin_SCL 5 // GPIO5
/********************* PWM ****************/
#define def_pin_PWM 18 // GPIO18
/***************** Write 4@20 mA **********/
#define def_pin_W4a20_1 26 // GPIO26
/************* BUTTONS GPIO define *********/
#define def_pin_RTN1 0   // GPIO0
#define def_pin_RTN2 4   // GPIO4
#define def_pin_PUSH1 16 // GPIO16
#define def_pin_PUSH2 17 // GPIO17

WifiManager_c wm;
Telnet_c WSerial(4000);
// UDP_c WSerial(47269);
// Hart_c ds8500Serial(4001);

Btn_c rtn_1(def_pin_RTN1);
Btn_c rtn_2(def_pin_RTN2);
Btn_c push_1(def_pin_PUSH1);
Btn_c push_2(def_pin_PUSH2);

// Use ESP, InIndKit, WiFi, ArduinoOTA, InIndKit.Display e InIndKit.Telnet
class InIndKit_c : public OTA_c, public Display_c
{
public:
    void setup(const char *DDNSName);
    void loop(void);
    void errorMsg(String error, bool restart = true);
};
#endif

inline void InIndKit_c::setup(const char *DDNSName)
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
    /***************** Read 4@20 mA ***********/
    pinMode(def_pin_R4a20_1, INPUT);
    pinMode(def_pin_R4a20_2, INPUT);
    /********************* OUT ****************/
    pinMode(def_pin_OUT1, OUTPUT);
    pinMode(def_pin_OUT2, OUTPUT);
    pinMode(def_pin_OUT3, OUTPUT);
    pinMode(def_pin_OUT4, OUTPUT);
    /********************* PWM ****************/
    pinMode(def_pin_PWM, OUTPUT);
    /********************* DAC ****************/
    pinMode(def_pin_DAC1, OUTPUT);
    /********************* RELÊ ***************/
    pinMode(def_pin_RELE, OUTPUT);
    /***************** Write 4@20 mA **********/
    pinMode(def_pin_W4a20_1, OUTPUT);

    if (displayStart(def_pin_SDA, def_pin_SCL))
    {
        setDisplayText(1, "Inicializando...");        
        Serial.println("Display running");
    }
    else errorMsg("Display error.", false);
    delay(50);
    
    // if (wifiStart(ssid, password)) // Primeiro o Wifi
    WiFi.mode(WIFI_STA);
    wm.setApName(DDNSName);       
    if (wm.autoConnect("AutoConnectAP"))
    {
        Serial.print("\nWifi running - IP:");
        Serial.println(WiFi.localIP());
        setDisplayText(1, WiFi.localIP().toString().c_str());
    }
    else
    {
        setFuncMode(true);
        setDisplayText(1, "Acces Point ON", true);
        errorMsg("Wifi  error.\nAP MODE...",false);
    }
    setDisplayText(2, DDNSName);
    setDisplayText(3, "UFU Mode"); 
    delay(50);    

    // if (!MDNS.begin(DDNSName))
    //{
    //     errorMsg("MDNS Error.\nWill reboot...");
    // }
    // MDNS.addService("http", "tcp", 80);

    otaStart(DDNSName); // Depois o OTA

    if (WSerial.start())
    {
        Serial.print("Telnet running - port:");
        Serial.println(WSerial.serverPort());
    }
    else errorMsg("WSerial  error.\n",false);

    push_1.setTimePressedButton(3);
    push_1.onPressedWithTime([this]()
    {
        if(wm.changeWebPortal())
        {
            ((Display_c *) this)->setFuncMode(true);
            ((Display_c *) this)->setDisplayText(2, "Web Portal ON", true);
            ((Display_c *) this)->setDisplayText(3, "", true);        
            //digitalWrite(def_pin_OUT1, LOW);            
        } else {
            ((Display_c *) this)->setFuncMode(false);
            //digitalWrite(def_pin_OUT1, HIGH); 
        }
    });
    // ds8500Serial.setup();

    // attachInterrupt(rtn_1.pin, interruptFunc, CHANGE);
    // attachInterrupt(rtn_2.pin, interruptFunc, CHANGE);
    // attachInterrupt(push_1.pin, interruptFunc, CHANGE);
    // attachInterrupt(push_2.pin, interruptFunc, CHANGE);

    digitalWrite(def_pin_OUT1, HIGH);
    digitalWrite(def_pin_OUT2, HIGH);
    digitalWrite(def_pin_OUT3, HIGH);
    digitalWrite(def_pin_OUT4, HIGH);
    digitalWrite(def_pin_RELE, HIGH);

    analogWrite(def_pin_PWM, 0);
    analogWrite(def_pin_DAC1, 0);
    analogWrite(def_pin_W4a20_1, 0);
}

void InIndKit_c::loop(void)
{
    ArduinoOTA.handle();
    WSerial.update();
    displayUpdate();
    if(wm.getPortalRunning()){
        wm.process();
    }
    // ds8500Serial.loop();
    rtn_1.update();
    rtn_2.update();
    push_1.update();
    push_2.update();
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