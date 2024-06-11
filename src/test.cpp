#include <Arduino.h>
#include "InIndKit.h"

void setup()
{
    InIndKit.setup("Wokwi-GUEST" , "", "inindkit0");
}

void loop()
{
    InIndKit.loop();
}