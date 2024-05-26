#include <Arduino.h>
#include "InIndKit.h"

unsigned long markTime = 0;
float i = 0;

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

void setup()
{
  InIndKit.start();
}

void loop()
{
  if (millis() - markTime > 50)
  {
    markTime = millis();
    i += 0.1;
    // Print log
    //Telnet.print("casa");
    //Telnet.println(i);

    // Plot a sinus
    Telnet.print(">sin:");
    Telnet.print(i);
    Telnet.print(":");
    Telnet.print(sin(i));
    Telnet.println("§Volts|g");

    // Plot a cosinus
    Telnet.print(">cos:");
    Telnet.print(i);
    Telnet.print(":");
    Telnet.print(cos(i));
    Telnet.println("§Volts|g");
  }
  InIndKit.update();
  monitoraBTN();
}
