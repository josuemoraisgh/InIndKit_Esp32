#include <Arduino.h>
#include "InIndKit.h"

AsyncDelay_c delayPlot(50); // time mili second
float timeStampsPlot = 0.0;
void printPlot(void);

AsyncDelay_c delayPOT(500); // time mili second
void monitoraPOT(void);

void monitoraBTN(void);
void telnetRead(String str);

void setup()
{
  InIndKit.start("Wokwi-GUEST","");// ssid, password ("NetMorais","32154538");
  Telnet.onInputReceived(telnetRead);
}

void loop()
{
  InIndKit.loop();
  monitoraBTN();
  monitoraPOT();
  printPlot();
}

void telnetRead(String str)
{
  if (str == "ping") // checks for a certain command
  {
    Telnet.println("pong");
    Serial.println("- Telnet: pong");
  }
  else if (str == "bye") // disconnect the client
  {
    Telnet.println("disconnecting you...");
    Telnet.disconnectClient();
  }
  else
  {
    Telnet.println(str);
  }
}

void monitoraBTN(void)
{
  if (debounceBtn(&InIndKit.rtn_1)) // Checa se o botao alterou sem debounce e se sim mudar o seu valor de status
  {
    Serial.println(InIndKit.rtn_1.status_btn ? "BotaoRTN1 ON" : "BotaoRTN1 OFF");
  }
  if (debounceBtn(&InIndKit.rtn_2))
    Serial.println(InIndKit.rtn_2.status_btn ? "BotaoRTN2 ON" : "BotaoRTN2 OFF");
  if (debounceBtn(&InIndKit.push_1))
    Serial.println(InIndKit.push_1.status_btn ? "BotaoPUSH1 ON" : "BotaoPUSH1 OFF");
  if (debounceBtn(&InIndKit.push_2))
    Serial.println(InIndKit.push_2.status_btn ? "BotaoPUSH2 ON" : "BotaoPUSH2 OFF");
}

void monitoraPOT(void)
{
  if (delayPOT.isExpired())
  {
    delayPOT.repeat();

    const uint16_t vlPOT_LEFT = analogRead(def_pin_POT_LEFT);
    InIndKit.setDisplayText(2, String(vlPOT_LEFT).c_str());
    Telnet.plot("pot_LEFT",vlPOT_LEFT,"V");
  
    const uint16_t vlPOT_RIGHT = analogRead(def_pin_POT_RIGHT);
    InIndKit.setDisplayText(3, String(vlPOT_RIGHT).c_str());
    Telnet.plot("pot_RIGHT",vlPOT_RIGHT,"V");    
  }
}

void printPlot(void)
{
  if (delayPlot.isExpired())
  {
    delayPlot.repeat();
    timeStampsPlot += 0.1;
    Telnet.plot("sin",timeStampsPlot,sin(timeStampsPlot));// Plot a sinus
    Telnet.plot("cos",timeStampsPlot,cos(timeStampsPlot));// Plot a cosinus
  }
}