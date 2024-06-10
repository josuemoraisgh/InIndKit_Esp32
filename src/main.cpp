#include <Arduino.h>
#include "InIndKit.h"

AsyncDelay_c delayPOT(500); // time mili second
void monitoraPOT(void);

void setup()
{
  InIndKit.setup("InovaIndustria", "industria50"); //("Wokwi-GUEST","");// ssid, password ;
  WSerial.onInputReceived([](String str){WSerial.println(str);});
  rtn_1.onValueChanged([](bool status){WSerial.println(status? "RTN1 ON" :"RTN1 OFF");});
  rtn_2.onValueChanged([](bool status){WSerial.println(status? "RTN1 ON" :"RTN1 OFF");});
  push_1.onValueChanged([](bool status){WSerial.println(status? "PUSH_1 ON" :"PUSH_1 OFF");});
  push_2.onValueChanged([](bool status){WSerial.println(status? "PUSH_2 ON" :"PUSH_2 OFF");});      
}

void loop()
{
  InIndKit.loop();
  monitoraPOT();
}

void monitoraPOT(void)
{
  if (delayPOT.isExpired())
  {
    delayPOT.repeat();

    const uint16_t vlPOT_LEFT = analogRead(def_pin_POT_LEFT);
    InIndKit.setDisplayText(2, String(vlPOT_LEFT).c_str());
    WSerial.plot("pot_LEFT", vlPOT_LEFT);

    const uint16_t vlPOT_RIGHT = analogRead(def_pin_POT_RIGHT);
    InIndKit.setDisplayText(3, String(vlPOT_RIGHT).c_str());
    WSerial.plot("pot_RIGHT", vlPOT_RIGHT);
  }
}