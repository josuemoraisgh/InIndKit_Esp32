#include <Arduino.h>
#include "IiKit.h"

typedef struct
{
  uint8_t pin;
  TickType_t delay;
} ledParameter_t;


class AsyncBlink
{
protected:
  ledParameter_t *ledParameter;
  static void toggleLED(void *);

public:
  AsyncBlink(uint8_t pin, TickType_t delay);
  void setPin(uint8_t pin) {this->ledParameter->pin = pin;}
  void setDelay(TickType_t delay) {this->ledParameter->delay = delay;}
};

void AsyncBlink::toggleLED(void *parameter) // Faz a mudança de estado de um led
{
  ledParameter_t *lp = (ledParameter_t *) parameter;
  uint8_t pinOld = lp->pin;
  for (;;)
  {
    digitalWrite(lp->pin, !digitalRead(lp->pin));
    vTaskDelay(lp->delay / portTICK_PERIOD_MS);
    if(pinOld != lp->pin) {
      digitalWrite(pinOld, LOW);
      pinOld = lp->pin;
    }
  }
}

AsyncBlink::AsyncBlink(uint8_t pin, TickType_t delay)
{
  this->ledParameter = new ledParameter_t({pin,delay});
  xTaskCreate(
      toggleLED,    // Function name
      "Task Blink",  // Task name
      1000,         // Stack size
      this->ledParameter, // Task parameters
      1,            // Task priority
      NULL          // Task handle
  );
}