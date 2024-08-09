#include "IiKit.h"
#include <Arduino.h>

typedef struct
{
  uint8_t pin;
  TickType_t delay;
} ledParameter_t;

void toggleLED(void *pinParameter) // Faz a mudança de estado de um led
{
  const ledParameter_t ledParameter = *(ledParameter_t *)pinParameter;
  for (;;)
  {
    digitalWrite(ledParameter.pin, !digitalRead(ledParameter.pin));
    vTaskDelay(ledParameter.delay / portTICK_PERIOD_MS);
  }
}

AsyncDelay_c delayPOT(50); // time mili second
void monitoraPOT(void)
{
  if (delayPOT.isExpired())
  {
    delayPOT.repeat();

    const uint16_t vlPOT1 = analogRead(def_pin_POT1);
    const uint16_t vlPOT2 = analogRead(def_pin_POT2);
    const uint16_t vlR4a20_1 = analogRead(def_pin_R4a20_1);
    const uint16_t vlR4a20_2 = analogRead(def_pin_R4a20_2);

    IIKit.disp.setText(2, ("P1:" + String(vlPOT1) + "  P2:" + String(vlPOT2)).c_str());
    IIKit.disp.setText(3, ("T1:" + String(vlR4a20_1) + "  T2:" + String(vlR4a20_2)).c_str());    

    IIKit.WSerial.plot("vlPOT1", vlPOT1);
    IIKit.WSerial.plot("vlPOT2", vlPOT2);
    IIKit.WSerial.plot("vlR4a20_1", vlR4a20_1);
    IIKit.WSerial.plot("vlR4a20_2", vlR4a20_2);    
  }
}

void setup()
{
  IIKit.setup();

  pinMode(def_pin_D1, OUTPUT);
  pinMode(def_pin_D2, OUTPUT);
  pinMode(def_pin_D3, OUTPUT);
  pinMode(def_pin_D4, OUTPUT);
  
  ledParameter_t led1 = {def_pin_D2, 500};
  
  xTaskCreate(
      toggleLED,   // Function name
      "Task LED1", // Task name
      1000,        // Stack size
      &led1,       // Task parameters
      1,           // Task priority
      NULL         // Task handle
  );

  IIKit.WSerial.onInputReceived([](const std::string &str) {
      if(str == "^q") IIKit.WSerial.stop(); 
      else IIKit.WSerial.print(str.c_str()); 
    }
  );

  IIKit.rtn_1.onValueChanged([](uint8_t status) {
      digitalWrite(def_pin_D1,status);
      IIKit.WSerial.println(status? "RTN1 ON" :"RTN1 OFF"); 
    }
  );
}

void loop()
{
  IIKit.loop();
  monitoraPOT();
}