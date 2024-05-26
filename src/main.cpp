#include <Arduino.h>
#include "InIndKit.h"

void monitoraBTN(void)
{
  if (debounceBtn(&InIndKit.rtn_1)) // Checa se o botao alterou sem debounce e se sim mudar o seu valor de status
    if (InIndKit.rtn_1.status_btn)  // Se o valor do status for verdadeiro
      Telnet.println("BotaoRTN1 ON");
    else
      Telnet.println("BotaoRTN1 OFF");
  if (debounceBtn(&InIndKit.rtn_2))
    if (InIndKit.rtn_2.status_btn)
      Telnet.println("BotaoRTN2 ON");
    else
      Telnet.println("BotaoRTN2 OFF");
  if (debounceBtn(&InIndKit.push_1))
    if (InIndKit.push_1.status_btn)
      Telnet.println("BotaoPUSH1 ON");
    else
      Telnet.println("BotaoPUSH1 OFF");
  if (debounceBtn(&InIndKit.push_2))
    if (InIndKit.push_2.status_btn)
      Telnet.println("BotaoPUSH2 ON");
    else
      Telnet.println("BotaoPUSH2 OFF");
}

void taskFunc1(void *)
{
  float i = 0;
  for (;;)
  {
    i += 0.1;

    // Print log
    Telnet.print("casa");
    Telnet.println(i);

    // Plot a sinus
    Telnet.print(">sin:");
    Telnet.println(sin(i));

    // Plot a cosinus
    Telnet.print(">Sum:");
    Telnet.println(0.8 * sin(i) + 0.2 * cos(i));

    delay(50);
  }
}

void setup()
{
  InIndKit.start();
  xTaskCreate(
      taskFunc1,     // Function name
      "Task Func 1", // Task name
      1000,          // Stack size
      NULL,          // Task parameters
      1,             // Task priority
      NULL           // Task handle
  );
}

void loop()
{
  InIndKit.update();
  monitoraBTN();
}
