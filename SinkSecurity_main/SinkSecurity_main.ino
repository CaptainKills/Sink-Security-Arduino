//Project Libraries
#include <RH_ASK.h> //Transmitter/Reciever library
#include <SPI.h> //Support library for TX/RX library

//Constants
#define WATER_PIN A1
const char *LEVEL_0 = "LEVEL_0";
const char *LEVEL_1 = "LEVEL_1";
const char *LEVEL_2 = "LEVEL_2";
const char *LEVEL_3 = "LEVEL_3";
const char *LEVEL_4 = "LEVEL_4";
#define INPUT_MARGIN 25

//Variable Objects
RH_ASK driver;

void setup() {
  //Serial Debugging Setup
  Serial.begin(9600);

  //Driver initialisation
  if(!driver.init()){
    Serial.println("Driver initialisation failed!");
  }

  //PinMode initialisation
  pinMode(INPUT, WATER_PIN);
}

void loop() {
  int input = analogRead(WATER_PIN); //can range from 0-1024
  Serial.println(input);

  //Check the ranges of the incoming input
  if(input <= 0 + INPUT_MARGIN){ //Level 0: do not send anything
    
  } else if(input <= 256 + INPUT_MARGIN){ //Level 0: do not send anything
    sendMessage(LEVEL_1);
    
  } else if(input <= 512 + INPUT_MARGIN){ //Level 1: do not send anything
    sendMessage(LEVEL_2);
    
  } else if(input <= 768 + INPUT_MARGIN){ //Level 2: do not send anything
    sendMessage(LEVEL_3);
    
  } else if(input <= 1024 + INPUT_MARGIN){ //Level 3: do not send anything
    sendMessage(LEVEL_4);
    
  }
}

void sendMessage(char *msg){
  driver.send((uint8_t *) msg, strlen(msg));
  driver.waitPacketSent(); 
}
