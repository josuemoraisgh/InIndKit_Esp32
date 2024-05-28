#include <Arduino.h>
#include "InIndKit.h"

unsigned long setTimePlot = 0;
float timeStampsPlot = 0;

void printPlot(void)
{
  if (millis() - setTimePlot > 50)
  {
    setTimePlot = millis();
    timeStampsPlot += 0.1;

    // Plot a sinus
    Telnet.print(">sin:");
    Telnet.print(timeStampsPlot);
    Telnet.print(":");
    Telnet.print(sin(timeStampsPlot));
    Telnet.println("§Volts|g");

    // Plot a cosinus
    Telnet.print(">cos:");
    Telnet.print(timeStampsPlot);
    Telnet.print(":");
    Telnet.print(cos(timeStampsPlot));
    Telnet.println("§Volts|g");
  }
}

unsigned long setTimePOT = 0;
float timeStampsPOT = 0;

void monitoraPOT(void)
{
  if (millis() - setTimePOT > 500)
  {
    setTimePOT = millis();
    timeStampsPOT += 0.1;

    const uint16_t vlPOT_LEFT = analogRead(def_pin_POT_LEFT);
    InIndKit.setDisplayText(2, String(vlPOT_LEFT).c_str());
    Telnet.print(">POT_LEFT:");
    Telnet.print(timeStampsPOT);
    Telnet.print(":");
    Telnet.print(vlPOT_LEFT);
    Telnet.println("§Volts|g");

    const uint16_t vlPOT_RIGHT = analogRead(def_pin_POT_RIGHT);
    InIndKit.setDisplayText(3, String(vlPOT_RIGHT).c_str());
    Telnet.print(">POT_RIGHT:");
    Telnet.print(timeStampsPOT);
    Telnet.print(":");
    Telnet.print(vlPOT_RIGHT);
    Telnet.println("§Volts|g");
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
  InIndKit.update();
  monitoraBTN();
  monitoraPOT();
  printPlot();
}
