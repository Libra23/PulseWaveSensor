#include <MsTimer2.h>

#define SAMPLING_RATE 10  // Hz

void getAnalogValue() {
  
}

void setup() {
  // put your setup code here, to run once:
  MsTimer2::set(500, intervalFunction);  // Set function called every set rate
  MsTimer2::start();          // Starts timer interrupt
}

void loop() {
  // put your main code here, to run repeatedly:

}
