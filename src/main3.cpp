#include <Arduino.h>
#include "IiKit.h"

#define FREQ 50.0
#define TIME_DELAY 10.0 // Time em microsegundos

#define CILCE_FREQ (FREQ * TIME_DELAY / 1000000.0)
#define CILCE_PERIODO (1.0 / CILCE_FREQ)

uint16_t vlPOT1 = 0;
uint16_t vlPOT2 = 0;

void plotWave(void *)
{
  uint16_t timeWave = 0;                            // Índice para percorrer a tabela de formas de onda
  AsyncDelay_c delayPlotWave(TIME_DELAY, ISMICROS); // time in micro second
  for (;;)
  {
    if (delayPlotWave.isExpired())
    {
      delayPlotWave.repeat();
      const double aux = (vlPOT2 * TIME_DELAY / 1000000.0);
      dacWrite(def_pin_DAC1, 127 + uint8_t(vlPOT1 * sin(2.0 * PI * aux * timeWave)));
      if (++timeWave >= uint16_t(1/aux)) timeWave = 0;
    }
  }
}

AsyncDelay_c delayPOT(50, ISMILI); // time mili second
void monitoraPOT(void)
{
  if (delayPOT.isExpired())
  {
    delayPOT.repeat();
    vlPOT1 = map(analogRead(def_pin_POT1), 0, 4095, 0, 127/*255*/);
    vlPOT2 = map(analogRead(def_pin_POT2), 0, 4095, 0, long(2*FREQ));
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
  monitoraPOT();
}