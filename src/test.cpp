#include <Arduino.h>
#include "InIndKit.h"

void setup()
{
    InIndKit.start("Wokwi-GUEST","");
}

void loop()
{
    InIndKit.loop();
}