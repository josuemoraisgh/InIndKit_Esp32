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
  InIndKit.start();
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
  InIndKit.update();
  if (debounceBtn(&InIndKit.rtn_1)) // Checa se o botao alterou sem debounce e se sim mudar o seu valor de status
    if (InIndKit.rtn_1.status_btn) // Se o valor do status for verdadeiro
      Telnet.print("BotaoRTN1 Ativo");
  if (debounceBtn(&InIndKit.rtn_2))
    if (InIndKit.rtn_2.status_btn)
      Telnet.print("BotaoRTN2 Ativo");
  if (debounceBtn(&InIndKit.push_1))
    if (InIndKit.push_1.status_btn)
      Telnet.print("BotaoPUSH1 Ativo");
  if (debounceBtn(&InIndKit.push_2))
    if (InIndKit.push_2.status_btn)
      Telnet.print("BotaoPUSH2 Ativo");
}
