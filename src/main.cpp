#include <Arduino.h>
#include "InIndKit.h"

AsyncDelay_c delayPOT(50); // time mili second
void monitoraPOT(void)
{
  if (delayPOT.isExpired())
  {
    delayPOT.repeat();

    const uint16_t vlPOT_LEFT = analogRead(def_pin_POT_LEFT);
    const uint16_t vlPOT_RIGHT = analogRead(def_pin_POT_RIGHT);
    InIndKit.setDisplayText(2, ("PL:" + String(vlPOT_LEFT) + "  PR:" + String(vlPOT_RIGHT)).c_str());
    WSerial.plot("pot_LEFT", vlPOT_LEFT);
    WSerial.plot("vlPOT_RIGHT", vlPOT_RIGHT);
  }
}

AsyncDelay_c delay4A(50); // time mili second
void monitora4A(void)
{
  if (delay4A.isExpired())
  {
    delay4A.repeat();

    const uint16_t vlR4a20_1 = analogRead(def_pin_R4a20_1);
    const uint16_t vlR4a20_2 = analogRead(def_pin_R4a20_2);
    InIndKit.setDisplayText(3, ("R1:" + String(vlR4a20_1) + "  R2:" + String(vlR4a20_2)).c_str());
    WSerial.plot("vlR4a20_1", vlR4a20_1);
    WSerial.plot("vlR4a20_2", vlR4a20_2);
  }
}

void setup()
{
  InIndKit.setup("inindkit0"); //("Wokwi-GUEST","","inindkit0");// ssid, password, DDNSName ;
  WSerial.onInputReceived([](String str)
                          { WSerial.println(str); });
  rtn_1.onValueChanged([](uint8_t status)
                       {digitalWrite(def_pin_OUT1,!status);WSerial.println(status? "RTN1 ON" :"RTN1 OFF"); });
  rtn_2.onValueChanged([](uint8_t status)
                       {digitalWrite(def_pin_OUT2,!status);WSerial.println(status? "RTN2 ON" :"RTN2 OFF"); });
  push_1.onValueChanged([](uint8_t status)
                        {digitalWrite(def_pin_OUT3,!status);WSerial.println(status? "PUSH_1 ON" :"PUSH_1 OFF"); });
  push_2.onValueChanged([](uint8_t status)
                        {digitalWrite(def_pin_OUT4,!status);WSerial.println(status? "PUSH_2 ON" :"PUSH_2 OFF"); });
}

void loop()
{
  InIndKit.loop();
  monitoraPOT();
  monitora4A();
}