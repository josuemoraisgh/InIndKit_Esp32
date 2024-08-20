#include <Arduino.h>
#include "IiKit.h"

AsyncDelay_c delayPOT(50, ISMILI); // time mili second
void monitoraPOT(void)
{
  if (delayPOT.isExpired())
  {
    delayPOT.repeat();

    const uint16_t vlPOT1 = analogRead(def_pin_POT1);
    const uint16_t vlPOT2 = analogRead(def_pin_POT2);

    IIKit.disp.setText(3, ("P1:" + String(vlPOT1) + "  P2:" + String(vlPOT2)).c_str());    

    IIKit.WSerial.plot("vlPOT1", vlPOT1);
    IIKit.WSerial.plot("vlPOT2", vlPOT2); 
  }
}