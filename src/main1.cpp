#include <Arduino.h>
#include "IiKit.h"

void plotWave(void *)
{
  AsyncDelay_c delayPlotWave(TIME_DELAY, ISMICROS); // time in micro second
  for (;;)
  {
    if (delayPlotWave.isExpired())
    {
      delayPlotWave.repeat();
      const uint16_t vlPOT1 = map(analogRead(def_pin_POT1), 0, 4095, 0, 255);      
      dacWrite(def_pin_DAC1,vlPOT1);
    }
  }
}

void setup()
{
  IIKit.setup();

  xTaskCreate(
      plotWave,    // Function name
      "Task Wave", // Task name
      5000,        // Stack size
      NULL,        // Task parameters
      1,           // Task priority
      NULL         // Task handle
  );
}

void loop()
{
  IIKit.loop();
}