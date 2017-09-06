#include <MsTimer2.h>

#define ANALOG_PIN    A0
#define SAMPLING_RATE 100  // Hz
#define START_BYTE 0xFE
#define END_BYTE 0xFF

uint16_t data; // raw data
uint8_t data_flag;

void getPulseWave() {
  data = analogRead(ANALOG_PIN);
  data_flag = 1;
}

void setup() {
  // put your setup code here, to run once:
  
  // set up serial communication
  Serial.begin(9600);
  
  // set up for sampling pulse wave
  data_flag = 0;
  MsTimer2::set(1000/ SAMPLING_RATE, getPulseWave);  // set function called every set interval ms
  MsTimer2::start();          // start timer interrupt
}

void loop() {
  // put your main code here, to run repeatedly:
  if(data_flag) {
    // clear flag
    data_flag = 0;
    
    // transmit packets
    // START_BYTE | upper data 3 bits | lower data 7 bits | END_BYTE
    Serial.write(START_BYTE);
    Serial.write((data >> 7) & 0b00000111);
    Serial.write(data & 0b01111111);
    Serial.write(END_BYTE);
    
    //Serial.println(data);
  }
}
