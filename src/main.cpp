#include <Arduino.h>
#include "InIndKit.h"

unsigned long setTime = 0;
float i = 0;

void monitoraBTN(void)
{
  if (debounceBtn(&InIndKit.rtn_1)) // Checa se o botao alterou sem debounce e se sim mudar o seu valor de status
    Telnet.println(InIndKit.rtn_1.status_btn ? "BotaoRTN1 ON" : "BotaoRTN1 OFF");
  if (debounceBtn(&InIndKit.rtn_2))
    Telnet.println(InIndKit.rtn_2.status_btn ? "BotaoRTN2 ON" : "BotaoRTN2 OFF");
  if (debounceBtn(&InIndKit.push_1))
    Telnet.println(InIndKit.push_1.status_btn ? "BotaoPUSH1 ON" : "BotaoPUSH1 OFF");
  if (debounceBtn(&InIndKit.push_2))
    Telnet.println(InIndKit.push_2.status_btn ? "BotaoPUSH2 ON" : "BotaoPUSH2 OFF");
}

void setup()
{
  InIndKit.start();
}

void loop()
{
  InIndKit.update();
  monitoraBTN();

  if (millis() - setTime > 50)
  {
    setTime = millis();
    i += 0.1;

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
}
