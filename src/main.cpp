#include <Arduino.h>
#include "InIndKit.h"

AsyncDelay_c delayPlot(50); // time mili second
float timeStampsPlot = 0.0;

void printPlot(void)
{
  if (delayPlot.isExpired())
  {
    delayPlot.repeat();
    timeStampsPlot += 0.1;
    // Plot a sinus
    Telnet.println((String() + ">sin:" + timeStampsPlot + ":" + sin(timeStampsPlot) + "§Volts|g").c_str());
    // Plot a cosinus
    Telnet.println((String() + ">cos:" + timeStampsPlot + ":" + cos(timeStampsPlot) + "§Volts|g").c_str());
  }
}

AsyncDelay_c delayPOT(500); // time mili second
float timeStampsPOT = 0.0;

void monitoraPOT(void)
{
  if (delayPOT.isExpired())
  {
    delayPOT.repeat();
    timeStampsPOT += 0.1;

    const uint16_t vlPOT_LEFT = analogRead(def_pin_POT_LEFT);
    InIndKit.setDisplayText(2, String(vlPOT_LEFT).c_str());
    Telnet.println(String() + ">POT_LEFT:" + timeStampsPOT + ":" + vlPOT_LEFT + "§Volts|g");

    const uint16_t vlPOT_RIGHT = analogRead(def_pin_POT_RIGHT);
    InIndKit.setDisplayText(3, String(vlPOT_RIGHT).c_str());
    Telnet.println(String() + ">POT_RIGHT:" + timeStampsPOT + ":" + vlPOT_RIGHT + "§Volts|g");
  }
}

void monitoraBTN(void)
{
  if (debounceBtn(&InIndKit.rtn_1)) // Checa se o botao alterou sem debounce e se sim mudar o seu valor de status
    Telnet.println(InIndKit.rtn_1.status_btn ? "BotaoRTN1 ON" : "BotaoRTN1 OFF");
  if (debounceBtn(&InIndKit.rtn_2))
    Telnet.println(String(InIndKit.rtn_2.status_btn ? "BotaoRTN2 ON" : "BotaoRTN2 OFF"));
  if (debounceBtn(&InIndKit.push_1))
    Telnet.println(String(InIndKit.push_1.status_btn ? "BotaoPUSH1 ON" : "BotaoPUSH1 OFF"));
  if (debounceBtn(&InIndKit.push_2))
    Telnet.println(String(InIndKit.push_2.status_btn ? "BotaoPUSH2 ON" : "BotaoPUSH2 OFF"));
}

void setup()
{
  InIndKit.start();
}

void loop()
{
  InIndKit.loop();
  monitoraBTN();
  monitoraPOT();
  printPlot();
}
