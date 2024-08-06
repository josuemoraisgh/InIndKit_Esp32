#ifndef __ININDKIT_H
#define __ININDKIT_H

#include <HardwareSerial.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include <WiFi.h>

#include "services\WSerial_c.h"
#include "services\display_c.h"
#include "services\ota_c.h"
#include "services\wifi_c.h"
#include "services\UDP_c.h"
#include "services\hartudp_c.h"
#include "services\wifimanager_c.h"
#include "util/asyncDelay.h"
#include "util/btn.h"

//////////////////////////Lado Esquerdo///////////////////////
/********** POTENTIOMETERS GPIO define *****/
#define def_pin_POT1 36 // GPIO36
#define def_pin_POT2 39 // GPIO39
/********************* ADC ****************/
#define def_pin_ADC1 34 // GPIO34
/***************** Read 4@20 mA ***********/
#define def_pin_R4a20_1 33 // GPIO33
#define def_pin_R4a20_2 32 // GPIO32
/******************** Digitais **************/
#define def_pin_D1 13 // GPIO13 - Não funciona como entrada analogica somente digital
#define def_pin_D2 5  // GPIO5  - Não funciona como entrada analogica somente digital
#define def_pin_D3 14 // GPIO14 - Não funciona como entrada analogica somente digital
#define def_pin_D4 27 // GPIO27 - Não funciona como entrada analogica somente digital
/********************* DAC ****************/
#define def_pin_DAC1 25 // GPIO25
/***************** Write 4@20 mA **********/
#define def_pin_W4a20_1 26 // GPIO26
/**************** Hart Interface **********/
// #define def_pin_Hart_RXD 3  // Pino RX da ESP32 conectado ao pino RX do DS8500
// #define def_pin_Hart_TXD 1  // Pino TX da ESP32 conectado ao pino TX do DS8500
// #define def_pin_Hart_RTS 22 // Pino RTS da ESP32 conectado ao pino RTS do DS8500
// #define def_pin_Hart_CTS 19 // Pino CTS da ESP32 conectado ao pino CD do DS8500
//////////////////////////Lado Direito///////////////////////
/********************* RELÊ ***************/
#define def_pin_RELE 23 // GPIO23
/***************** OLED Display ************/
#define def_pin_SDA 21 // GPIO21
#define def_pin_SCL 22 // GPIO22
/********************* PWM ****************/
#define def_pin_PWM 18 // GPIO18
/************* BUTTONS GPIO define *********/
#define def_pin_RTN1 15  // GPIO15
#define def_pin_RTN2 35  // GPIO35
#define def_pin_PUSH1 16 // GPIO16
#define def_pin_PUSH2 17 // GPIO17

WifiManager_c wm;
// HartUdp_c ds8500Serial(4000);

Btn_c rtn_1(def_pin_RTN1);
Btn_c rtn_2(def_pin_RTN2);
Btn_c push_1(def_pin_PUSH1);
Btn_c push_2(def_pin_PUSH2);

// Use ESP, InIndKit, WiFi, ArduinoOTA, InIndKit.Display e InIndKit.Telnet
class InIndKit_c : public OTA_c, public Display_c
{
private:
    char DDNSName[15] = "inindkit";

public:
    void setup();
    void loop(void);
    void errorMsg(String error, bool restart = true);
};

inline void InIndKit_c::setup()
{
    WSerial.println("Booting");
    /*********** Inicializando Display ********/
    if (displayStart(def_pin_SDA, def_pin_SCL))
    {
        setDisplayText(1, "Inicializando...");
        WSerial.println("Display running");
    }
    else errorMsg("Display error.", false);
    delay(50);
    /*************** READ EEPROM *************/
    EEPROM.begin(1);
    char idKit[2] = "0";    
    /*************** Write EEPROM ************/
    //EEPROM.write(0,(uint8_t) idKit[0]);
    //EEPROM.commit();
    /********** Initializes with kit id ******/
    idKit[0] = (char)EEPROM.read(0); // id do kit utilizado
    strcat(DDNSName, idKit);
    /************** Starting WIFI ************/
    WiFi.mode(WIFI_STA);
    /********* Starting WIFI Manager *********/       
    wm.setApName(DDNSName);
    setFuncMode(true);
    setDisplayText(1, "Mode: Acces Point", true);
    setDisplayText(2, "SSID: AutoConnectAP", true);
    setDisplayText(3, "PSWD: ", true);
    if (wm.autoConnect("AutoConnectAP"))
    {
        WSerial.print("\nWifi running - IP:");
        WSerial.println(WiFi.localIP());
        setFuncMode(false);
        setDisplayText(1, WiFi.localIP().toString().c_str());
        setDisplayText(2, DDNSName);
        setDisplayText(3, "UFU Mode");
        delay(50);
    }
    else errorMsg("Wifi  error.\nAP MODE...", false);
    
    /************** Starting OTA *************/    
    otaStart(DDNSName);  // OTA tem que ser depois do wifi e wifiManager 
    /*** Starting Telnet Mode in WSerial ****/
    WSerial.telnetStart(4000);    
    /********** POTENTIOMETERS GPIO define *****/
    pinMode(def_pin_POT1, ANALOG);
    pinMode(def_pin_POT2, ANALOG);
    /************* BUTTONS GPIO define *********/
    pinMode(def_pin_RTN1, INPUT_PULLDOWN);
    pinMode(def_pin_RTN2, INPUT_PULLDOWN);
    pinMode(def_pin_PUSH1, INPUT_PULLDOWN);
    pinMode(def_pin_PUSH2, INPUT_PULLDOWN);
    /*************** IOs GPIO define **********/
    pinMode(def_pin_D1, OUTPUT);
    pinMode(def_pin_D2, OUTPUT);
    pinMode(def_pin_D3, OUTPUT);
    pinMode(def_pin_D4, OUTPUT);
    /***************** Read 4@20 mA ***********/
    pinMode(def_pin_R4a20_1, ANALOG);
    pinMode(def_pin_R4a20_2, ANALOG);
    /********************* PWM ****************/
    pinMode(def_pin_PWM, OUTPUT);
    /********************* DAC ****************/
    pinMode(def_pin_DAC1, ANALOG);
    /********************* ADC ****************/
    pinMode(def_pin_ADC1, ANALOG);
    /********************* RELÊ ***************/
    pinMode(def_pin_RELE, OUTPUT);
    /***************** Write 4@20 mA **********/
    pinMode(def_pin_W4a20_1, OUTPUT);
    /************ Web Portal ****************/
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
            ((Display_c *) this)->setDisplayText(2, DDNSName);            
            //digitalWrite(def_pin_OUT1, HIGH); 
        } });
    /************ HART MODEM ****************/        
    // ds8500Serial.setup(def_pin_Hart_RXD, def_pin_Hart_TXD, def_pin_Hart_CTS, def_pin_Hart_RTS);
    /************ OTHER CONFIG **************/
    digitalWrite(def_pin_D1, LOW);
    digitalWrite(def_pin_D2, LOW);
    digitalWrite(def_pin_D3, LOW);
    digitalWrite(def_pin_D4, LOW);
    digitalWrite(def_pin_RELE, LOW);
    analogWrite(def_pin_PWM, 0);
    analogWrite(def_pin_DAC1, 0);
    analogWrite(def_pin_W4a20_1, 0);
}

void InIndKit_c::loop(void)
{
    ArduinoOTA.handle();
    WSerial.update();
    displayUpdate();
    if (wm.getPortalRunning())
    {
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
    WSerial.println(error);
    if (restart)
    {
        WSerial.println("Rebooting now...");
        delay(2000);
        ESP.restart();
        delay(2000);
    }
}
InIndKit_c InIndKit;
#endif