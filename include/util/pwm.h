#include <Arduino.h>
#include "IiKit.h"
#include "driver/ledc.h"

//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html

const int dcc_pin = 19;               // choix de la broche dcc out : n'importe laquelle peut être utilisée
//                                    // en se servant de la matrice du gpio
//                                    // setting PWM properties
#define dcc_sig_1_freq 8621           // fréquency of ~116µs 
#define dcc_sig_0_freq 5000           // fréquency of 200µs ,

const int pin_channel = 0;            // choice of te channel PWM = 0
unsigned int sig_resolution = 1;      // resolution = 1 bit => 2^1 = 0,1,2
unsigned long sig_duty = 1;           // cyclacity of 1/2 or 50% 1 is 50% or 1/2^1
uint32_t sig_freq = dcc_sig_0_freq;   // set the frequency
int percent = 50;                     // Duty percentage
//---------------------------------------------------------------------------
void setup() {
//  Serial.begin(115200);
  delay(200);

  //  Calculation of settings for each frequency band
  //  Resolution = log2(Clock(80MHz)/f) + 1   ex: 50.000 HZ = 80.0000/50.000 = 1.600 log2(1600) = 10 + 1 = 11
  // wrong  //  Duty A%  = (2^Resolution)*(A/100)       ex: duty 50% --> 2**11 = 2048   2048/2 = 1024
  // correct//  Duty A%  = (2^Resolution)/(100/A)       ex: duty 50% --> 2**11 = 2048   2048/2 = 1024

  sig_resolution = int(log((80000000 / sig_freq) + 1) / log(2));
//  Serial.print("sig_resolution: "); Serial.println(sig_resolution);                 
  sig_duty = pow(2, sig_resolution)/(100/percent);
  
//  Serial.print("sig_duty: "); Serial.println(sig_duty); 

  ledcAttachPin(dcc_pin, pin_channel);                  // attach pin 19 to channel PWM
  ledcSetup(pin_channel, sig_freq, sig_resolution);     //freauency of 5000 or 8621
  ledcWrite(pin_channel, sig_duty );                    // set channel 0 with a cyclacity of 50%
}
//---------------------------------------------------------------------------
void loop() {
  delay(10);
}