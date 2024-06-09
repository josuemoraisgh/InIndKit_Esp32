#include <Arduino.h>
#include "InIndKit.h"

AsyncDelay_c delayPOT(500); // time mili second
void monitoraPOT(void);

void setup()
{
  InIndKit.setup("APJosue", "josue32154538"); //("Wokwi-GUEST","");// ssid, password ;
  Telnet.onInputReceived([](String str) 
    {
      Telnet.println(str);
    }
  );
  
  attachInterrupt(
      InIndKit.rtn_1.pin,
      []()
      {
        if (debounceBtn(&InIndKit.rtn_1)) // Checa se o botao alterou sem debounce e se sim mudar o seu valor de status
          Telnet.println(InIndKit.rtn_1.status_btn ? "BotaoRTN1 ON" : "BotaoRTN1 OFF");
      },
      CHANGE);
  attachInterrupt(
      InIndKit.rtn_2.pin,
      []()
      {
        if (debounceBtn(&InIndKit.rtn_2)) // Checa se o botao alterou sem debounce e se sim mudar o seu valor de status
          Telnet.println(InIndKit.rtn_2.status_btn ? "BotaoRTN2 ON" : "BotaoRTN2 OFF");
      },
      CHANGE);
  attachInterrupt(
      InIndKit.push_1.pin,
      []()
      {
        if (debounceBtn(&InIndKit.push_1)) // Checa se o botao alterou sem debounce e se sim mudar o seu valor de status
          Telnet.println(InIndKit.push_1.status_btn ? "BotaoPush_1 ON" : "BotaoPush_1 OFF");
      },
      CHANGE);
  attachInterrupt(
      InIndKit.push_2.pin,
      []()
      {
        if (debounceBtn(&InIndKit.push_2)) // Checa se o botao alterou sem debounce e se sim mudar o seu valor de status
          Telnet.println(InIndKit.push_2.status_btn ? "BotaoPush_2 ON" : "BotaoPush_2 OFF");
      },
      CHANGE);
}

void loop()
{
  InIndKit.loop();
  monitoraPOT();
}

void monitoraPOT(void)
{
  if (delayPOT.isExpired())
  {
    delayPOT.repeat();

    const uint16_t vlPOT_LEFT = analogRead(def_pin_POT_LEFT);
    InIndKit.setDisplayText(2, String(vlPOT_LEFT).c_str());
    Telnet.plot("pot_LEFT", vlPOT_LEFT);

    const uint16_t vlPOT_RIGHT = analogRead(def_pin_POT_RIGHT);
    InIndKit.setDisplayText(3, String(vlPOT_RIGHT).c_str());
    Telnet.plot("pot_RIGHT", vlPOT_RIGHT);
  }
}