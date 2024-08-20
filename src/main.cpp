#include <Arduino.h>
#include "IiKit.h"
#include "util/asyncWave.h"
#include "util/asyncBlink.h"
#include "util/monitPot.h"

void plotWave(void *)
{
  uint16_t waveIndex = 0;                 // Índice para percorrer a tabela de formas de onda
  AsyncDelay_c delayPlotWave(10, ISMICROS); // time in micro second
  for (;;)
  {
    if (delayPlotWave.isExpired())
    {
      delayPlotWave.repeat();
      dacWrite(def_pin_DAC1, 127 + uint8_t(127 * sin(2 * PI * waveIndex / 2000)));
      if (++waveIndex >= 2000) waveIndex = 0;
    }
  }
}

void setup()
{
  IIKit.setup();
  
  pinMode(def_pin_D1, OUTPUT);
  pinMode(def_pin_D2, OUTPUT);
  pinMode(def_pin_D3, OUTPUT);
  pinMode(def_pin_D4, OUTPUT);

  xTaskCreate(
      plotWave, // Function name
      "Task Wave",  // Task name
      5000,         // Stack size
      NULL,         // Task parameters
      1,            // Task priority
      NULL          // Task handle
  );
}

void loop()
{
  IIKit.loop();
  IIKit.WSerial.plot("ADC", analogRead(def_pin_ADC1));
}