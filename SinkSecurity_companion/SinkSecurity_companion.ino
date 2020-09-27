//Project Libraries
#include <RH_ASK.h>
#include <SPI.h>

//Constants
#define MESSAGE_LENGTH 12

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
  //Create space for message in memory
  char *message = (char *) malloc(MESSAGE_LENGTH);
  //Recieve the message
  message = recieveMessage();
  
  Serial.print("Message: ");
  Serial.prinln(message);

  //Free space in memory
  free(message);
}

char *recieveMessage(){
  uint8_t buf[MESSAGE_LENGTH];
  uint8_t buflen = sizeof(bug);

  if(driver.recv(buf, &buflen){
    return buf;
  } else{
    return null;
  }
}
