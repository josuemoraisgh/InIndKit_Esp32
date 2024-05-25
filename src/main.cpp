#include <Arduino.h>
#include "InIndKit.h"

/*void taskFunc1(void *)
{
  for (;;)
  {
  }
}*/

void setup()
{
  inIndKit.start();
  /*xTaskCreate(
      taskFunc1,    // Function name
      "Task Func 1", // Task name
      1000,          // Stack size
      NULL,          // Task parameters
      1,             // Task priority
      NULL           // Task handle
  );*/
}

void loop()
{
  inIndKit.update();
  if (debounceBtn(inIndKit.rtn_1)) // Checa se o botao alterou sem debounce e se sim mudar o seu valor de status
    if (inIndKit.rtn_1.status_btn) // Se o valor do status for verdadeiro
      inIndKit.telnet_o.print("BotaoRTN1 Ativo");
  if (debounceBtn(inIndKit.rtn_2))
    if (inIndKit.rtn_2.status_btn)
      inIndKit.telnet_o.print("BotaoRTN2 Ativo");
  if (debounceBtn(inIndKit.push_1))
    if (inIndKit.push_1.status_btn)
      inIndKit.telnet_o.print("BotaoPUSH1 Ativo");
  if (debounceBtn(inIndKit.push_2))
    if (inIndKit.push_2.status_btn)
      inIndKit.telnet_o.print("BotaoPUSH2 Ativo");
}
