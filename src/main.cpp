#include <Arduino.h>
#include "InIndKit.h"

InIndKit_c inIndKit_o;

void checkBotao(void *)
{
  for (;;)
  {
    if (debounceBtn(inIndKit_o.rtn_1)) //Checa se o botao alterou sem debounce e se sim mudar o seu valor de status
      if (inIndKit_o.rtn_1.status_btn) //Se o valor do status for verdadeiro 
        Serial.print("BotaoRTN1 Ativo");
    if (debounceBtn(inIndKit_o.rtn_2))
      if (inIndKit_o.rtn_1.status_btn)
        Serial.print("BotaoRTN2 Ativo");
    if (debounceBtn(inIndKit_o.push_1))
      if (inIndKit_o.rtn_1.status_btn)
        Serial.print("BotaoPUSH1 Ativo");
    if (debounceBtn(inIndKit_o.push_2))
      if (inIndKit_o.rtn_1.status_btn)
        Serial.print("BotaoPUSH2 Ativo");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  inIndKit_o.start();
  xTaskCreate(
      checkBotao,    // Function name
      "Task Analog", // Task name
      1000,          // Stack size
      NULL,          // Task parameters
      1,             // Task priority
      NULL           // Task handle
  );
}

void loop()
{
  inIndKit_o.update();
}
