#include <Arduino.h>
#include "IiKit.h"

#define FREQ 50.0
#define TIME_DELAY 10.0 // Time em microsegundos

#define CILCE_FREQ (FREQ * TIME_DELAY / 1000000.0)
#define CILCE_PERIODO (1.0 / CILCE_FREQ)

void plotWave(void *)
{
  uint16_t timeWave = 0;                            // Índice para percorrer a tabela de formas de onda
  AsyncDelay_c delayPlotWave(TIME_DELAY, ISMICROS); // time in micro second
  for (;;)
  {
    if (delayPlotWave.isExpired())
    {
      delayPlotWave.repeat();
      dacWrite(def_pin_DAC1, 127 + uint8_t(127 * sin(2.0 * PI * CILCE_FREQ * timeWave)));
      if (++timeWave >= CILCE_PERIODO) timeWave = 0;
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