#include <Arduino.h>
#include "InIndKit.h"

InIndKit_c  inIndKit_o; 

void setup() {
  inIndKit_o.start();
}

float i = 0;

void loop() {
  inIndKit_o.update();
  i += 0.1;

  // Print log
  inIndKit_o.telnet_o.telnet.print("casa");
  inIndKit_o.telnet_o.telnet.println(i);

  // Plot a sinus
  inIndKit_o.telnet_o.telnet.print(">sin:");
  inIndKit_o.telnet_o.telnet.println(sin(i));

  // Plot a cosinus
  inIndKit_o.telnet_o.telnet.print(">Sum:");
  inIndKit_o.telnet_o.telnet.println(0.8 * sin(i) + 0.2 * cos(i));

  delay(50);  
}
