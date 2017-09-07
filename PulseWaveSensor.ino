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


/*
 * processing program
import processing.serial.*;

static final String COM = "/dev/tty.usbserial-AH017L15";  // for mac
//static final String COM = "COM4";  // for windows
static final int DATA_NUM = 1;
static final int DATA_RESOLUTION = 10;  // bits
static final int RECEIVE_NUM = (DATA_RESOLUTION / 7 + 1) * DATA_NUM;
static final int TRANSMIT_NUM = 1;
static final int START_BYTE = 0xFE;
static final int END_BYTE = 0xFF;

static final int DISPLAY_NUM = 512;

static final int DISPLAY_X_PIXEL = 1024;  // pixel
static final int DISPLAY_Y_PIXEL = 512;  // pixel

static final int SAMPLING_RATE = 100; // Hz

Serial serial;
int[] data;
int data_index;
boolean data_flag, renew_flag;

int[] pulse_wave, pulse_view;
int[] pulse_view_velocity, pulse_view_accel;

void settings() {
  size(DISPLAY_X_PIXEL, DISPLAY_Y_PIXEL);
}

void setup() {
  // set up serial communication
  serial = new Serial(this, COM, 9600);
  data = new int[RECEIVE_NUM];
  data_index = 0;
  data_flag = false;
  // set up pulse wave form
  pulse_wave = new int[DISPLAY_NUM];
  pulse_view = new int[DISPLAY_NUM];
  pulse_view_velocity = new int[DISPLAY_NUM];
  pulse_view_accel = new int[DISPLAY_NUM];
}

void draw() {
  background(0);
  if(renew_flag) {
    renew_flag = false;
    renewData();
  }
  drawData();
}

void renewData() {
  for(int i = 0; i < pulse_wave.length - 1; i++) {
    pulse_wave[i] = pulse_wave[i + 1];
  }
  pulse_wave[pulse_wave.length - 1] = ((data[0] & 0x07) << 7) + (data[1] & 0x7F);
}

void drawData() {
  for(int i = 0; i < pulse_view.length; i++) {
    pulse_view[i] = pulse_wave[i] * DISPLAY_Y_PIXEL / (1 << DATA_RESOLUTION); // avoid int / int
  }
  for(int i = 0; i < pulse_view_velocity.length - 1; i++) {
    pulse_view_velocity[i] = (int)((pulse_view[i + 1] - pulse_view[i]) * SAMPLING_RATE);
  }
  for(int i = 0; i < pulse_view_accel.length - 2; i++) {
    pulse_view_accel[i] = (int)((pulse_view_velocity[i + 1] - pulse_view_velocity[i]) * SAMPLING_RATE);
  }
  strokeWeight(1);
  stroke( 0, 0, 255);  // draw line in blue
  for(int i = 0; i < pulse_wave.length - 1; i++) {
    line(i, DISPLAY_Y_PIXEL / 3 - pulse_view[i] / 3, i + 1, DISPLAY_Y_PIXEL / 3 - pulse_view[i + 1] / 3);
  }
  stroke( 0, 255, 0);  // draw line in blue
  for(int i = 0; i < pulse_wave.length - 2; i++) {
    line(i, DISPLAY_Y_PIXEL * 2 / 3 - pulse_view_velocity[i] / 30, i + 1, DISPLAY_Y_PIXEL *2 / 3 - pulse_view_velocity[i + 1] / 30);
  }
  stroke( 255, 0, 0);  // draw line in blue
  for(int i = 0; i < pulse_wave.length - 3; i++) {
    line(i, DISPLAY_Y_PIXEL - pulse_view_accel[i] / 30000, i + 1, DISPLAY_Y_PIXEL - pulse_view_accel[i + 1] / 30000);
  }
}

void serialEvent(Serial p) {
  int cmd = p.read();
    switch(cmd) {
  case START_BYTE:
    data_flag = true;
    data_index = 0;
    break;
  case END_BYTE:
    data_flag = false;
    renew_flag = true;
    break;
  default:
    if (data_flag == true) {
      data[data_index] = cmd;
      data_index++;
      if (data_index >= RECEIVE_NUM) {
        data_index = 0;
        data_flag = false;
        renew_flag = true;
      }
    }
    break;
  }
}
*/
