#include <Arduino.h>
#include "InIndKit.h"

void setup()
{
    InIndKit.setup("Wokwi-GUEST","");
}

void loop()
{
    InIndKit.loop();
}