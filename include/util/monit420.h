#include <Arduino.h>
#include "IiKit.h"

AsyncDelay_c delay420(500, ISMILI); // time mili second
void monitora420(void)
{
  if (delay420.isExpired())
  {
    delay420.repeat();

    const uint16_t vlR4a20_1 = (20.0 / 4095.0) * analogRead(def_pin_R4a20_1);
    const uint16_t vlR4a20_2 = (20.0 / 4095.0) * analogRead(def_pin_R4a20_2);

    IIKit.disp.setText(2, ("T1:" + String(vlR4a20_1) + "  T2:" + String(vlR4a20_2)).c_str());    

    IIKit.WSerial.plot("vlR4a20_1", vlR4a20_1);
    IIKit.WSerial.plot("vlR4a20_2", vlR4a20_2);    
  }
}