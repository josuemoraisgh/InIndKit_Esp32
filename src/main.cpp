#include "InIndKit.h"
#include <Arduino.h>

AsyncDelay_c delayPOT(50); // time mili second
void monitoraPOT(void)
{
  if (delayPOT.isExpired())
  {
    delayPOT.repeat();

    const uint16_t vlPOT1 = analogRead(def_pin_POT1);
    const uint16_t vlPOT2 = analogRead(def_pin_POT2);

    InIndKit.setDisplayText(2, ("P1:" + String(vlPOT1) + "  P2:" + String(vlPOT2)).c_str());
    
    WSerial.plot("vlPOT1", vlPOT1);
    WSerial.plot("vlPOT2", vlPOT2);
  }
}

AsyncDelay_c delay4A20(50); // time mili second
void monitora4A20(void)
{
  if (delay4A20.isExpired())
  {
    delay4A20.repeat();

    const double vlR4a20_1 = 20.0*analogRead(def_pin_R4a20_1)/4096;
    const double vlR4a20_2 = 20.0*analogRead(def_pin_R4a20_2)/4096;

    InIndKit.setDisplayText(3, ("T1:" + String(vlR4a20_1) + "  T2:" + String(vlR4a20_2)).c_str());
    
    WSerial.plot("vlR4a20_1", vlR4a20_1);
    WSerial.plot("vlR4a20_2", vlR4a20_2);
  }
}
void setup()
{
  InIndKit.setup();

  pinMode(def_pin_D1, OUTPUT);
  pinMode(def_pin_D2, OUTPUT);
  pinMode(def_pin_D3, OUTPUT);
  pinMode(def_pin_D4, OUTPUT);

  WSerial.onInputReceived([](String str) {
    if(str == "^q") WSerial.telnetStop(); 
    else WSerial.println(str); 
    }
  );
  
  rtn_1.onValueChanged([](uint8_t status) {
      digitalWrite(def_pin_D1,status);
      WSerial.println(status? "RTN1 ON" :"RTN1 OFF"); 
    }
  );
  rtn_2.onValueChanged([](uint8_t status) {
    digitalWrite(def_pin_D2,status);
    WSerial.println(status? "RTN2 ON" :"RTN2 OFF"); 
    }
  );
  push_1.onValueChanged([](uint8_t status) {
    digitalWrite(def_pin_D3,status);
    WSerial.println(status? "PUSH_1 ON" :"PUSH_1 OFF"); 
    }
  );
  push_2.onValueChanged([](uint8_t status) {
    digitalWrite(def_pin_D4,status);
    WSerial.println(status? "PUSH_2 ON" :"PUSH_2 OFF"); 
    }
  );
}

void loop()
{
  InIndKit.loop();
  monitoraPOT();
  monitora4A20();
}