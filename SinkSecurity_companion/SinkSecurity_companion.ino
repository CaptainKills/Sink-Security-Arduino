//Project Libraries
#include <RH_ASK.h> //Transmitter/Reciever library
#include <SPI.h>  //Support library for TX/RX library

//Constants
#define MESSAGE_LENGTH 7
#define LEVEL_0 "LEVEL_0"
#define LEVEL_1 "LEVEL_1"
#define LEVEL_2 "LEVEL_2"
#define LEVEL_3 "LEVEL_3"

//Variable Objects
RH_ASK driver;
char *message;

void setup() {
  //Serial Debugging Setup
  Serial.begin(9600);

  //Driver initialisation
  if(!driver.init()){
    Serial.println("Driver initialisation failed!");
  }

  //PinMode initialisation

}

void loop() {
  //Create space for message in memory
  message = (char *) malloc(MESSAGE_LENGTH);
  //Recieve the message
  message = recieveMessage();
  
  Serial.print("Message: ");
  Serial.println(message);

  //Check which message has been sent
  if(message == LEVEL_1){ //Level 1: light vibrations.
    Serial.println("Level 1 Detected: Set Vribration Level to ...");
    
  } else if(message == LEVEL_2){ //Level 2: medium vibrations.
    Serial.println("Level 2 Detected: Set Vribration Level to ...");
    
  } else if(message == LEVEL_3){ //Level 3: strong vibrations.
    Serial.println("Level 3 Detected: Set Vribration Level to ...");
   
  } else if(message == LEVEL_0){ //Level 0: disable vibrations.
    Serial.println("Level 0 Detected: Turning off.");
  } else{
    //Default case: no message revieved.
  }

  //Free space in memory
  free(message);
}

char *recieveMessage(){
  uint8_t buf[MESSAGE_LENGTH];
  uint8_t buflen = sizeof(buf);

  if(driver.recv(buf, &buflen)){
    return buf;
  } else{
    return "";
  }
}
