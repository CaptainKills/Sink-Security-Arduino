//Project Libraries
#include <RH_ASK.h>
#include <SPI.h>

//PIN Constants
#define WATER_PIN A1

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
  int input = analogRead(WATER_PIN);
  Serial.println(input);

  
}

void recieveMessage(char *msg){
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent(); 
}
