#include <Arduino.h>
#include "IiKit.h"
#include "wave.h"
#include "blink.h"

AsyncDelay_c delayPOT(50); // time mili second
void monitoraPOT(void)
{
  if (delayPOT.isExpired())
  {
    delayPOT.repeat();

    const uint16_t vlPOT1 = analogRead(def_pin_POT1);
    const uint16_t vlPOT2 = analogRead(def_pin_POT2);
    const uint16_t vlR4a20_1 = (20.0 / 4095.0) * analogRead(def_pin_R4a20_1);
    const uint16_t vlR4a20_2 = (20.0 / 4095.0) * analogRead(def_pin_R4a20_2);

    IIKit.disp.setText(2, ("P1:" + String(vlPOT1) + "  P2:" + String(vlPOT2)).c_str());
    IIKit.disp.setText(3, ("T1:" + String(vlR4a20_1) + "  T2:" + String(vlR4a20_2)).c_str());    

    IIKit.WSerial.plot("vlPOT1", vlPOT1);
    IIKit.WSerial.plot("vlPOT2", vlPOT2);
    IIKit.WSerial.plot("vlR4a20_1", vlR4a20_1);
    IIKit.WSerial.plot("vlR4a20_2", vlR4a20_2);    
  }
}

//waveParameter_t waveParameter = {def_pin_DAC1, 1, 1}; 
BlinkLED bLED(def_pin_D1,500);
void setup()
{
  IIKit.setup();
  pinMode(def_pin_D1, OUTPUT);
  pinMode(def_pin_D2, OUTPUT);
  pinMode(def_pin_D3, OUTPUT);
  pinMode(def_pin_D4, OUTPUT);

  IIKit.WSerial.onInputReceived([](const std::string &str) {
      if(str == "^q") IIKit.WSerial.stop(); 
      else IIKit.WSerial.print(str.c_str()); 
    }
  );

  IIKit.rtn_1.onValueChanged([](uint8_t status) {
      if(status) bLED.setPin(def_pin_D2);
      else bLED.setPin(def_pin_D1);
    }
  );  
}

void loop()
{
  IIKit.loop();
  monitoraPOT();
}