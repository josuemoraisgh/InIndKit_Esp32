#include <Arduino.h>
#include "IiKit.h"
#include "util/asyncWave.h"
#include "util/asyncBlink.h"
#include "util/monit420.h"
#include "util/monitPot.h"

AsyncBlink bLED(def_pin_D1,500);
//AsyncWave wv(def_pin_DAC1,60,0);

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

uint8_t waveIndex = 0; // Índice para percorrer a tabela de formas de onda  
AsyncDelay_c delayWave(1); // time mili second
void plotWave(void)
{
  if (delayWave.isExpired())
  {
    delayWave.repeat();
    dacWrite(def_pin_DAC1, 127+127*sin(2*PI*50*waveIndex));
    if (++waveIndex >= 20) waveIndex = 0;   
  }
}

void loop()
{
  IIKit.loop();
  monitoraPOT();
  monitora420();
  plotWave(); 
}