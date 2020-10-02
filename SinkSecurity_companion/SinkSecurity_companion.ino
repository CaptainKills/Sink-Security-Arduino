//Project Libraries
#include <RH_ASK.h> //Transmitter/Reciever library
#include <SPI.h>  //Support library for TX/RX library

//Constants
#define MOTOR_PIN A0
#define MESSAGE_LENGTH 7
const char *LEVEL_0 = "LEVEL_0";
const char *LEVEL_1 = "LEVEL_1";
const char *LEVEL_2 = "LEVEL_2";
const char *LEVEL_3 = "LEVEL_3";
const char *LEVEL_4 = "LEVEL_4";

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
  pinMode(OUTPUT, A0);
}

void loop() {
  //Create space for message in memory
  message = (char *) malloc(MESSAGE_LENGTH * sizeof(char));
  //Recieve the message
  message = recieveMessage();
  
  Serial.print("Message: ");
  Serial.println(message);

  //Check which message has been sent
  if(strcmp(message, LEVEL_1)){ //Level 1: light vibrations.
    Serial.println("Level 1 Detected: Set Vribration Level to 1");
    setMotorLevel(1);
    
  } else if(strcmp(message, LEVEL_2) == 0){ //Level 2: medium vibrations.
    Serial.println("Level 2 Detected: Set Vribration Level to 2");
    setMotorLevel(2);
    
  } else if(strcmp(message, LEVEL_3) == 0){ //Level 3: strong vibrations.
    Serial.println("Level 3 Detected: Set Vribration Level to 3");
    setMotorLevel(3);
   
  } else if(strcmp(message, LEVEL_4) == 0){ //Level 4: strongest vibrations
    Serial.println("Level 4 Detected: Set Vribration Level to 4");
    setMotorLevel(4);
  } else if(strcmp(message, LEVEL_0) == 0){ //Level 0: disable vibrations.
    Serial.println("Level 0 Detected: Turning off.");
    setMotorLevel(0);
    
  } else{
    //Default case: no message revieved.
  }

  //Free space in memory
  free(message);
}

void setMotorLevel(int level){
  int pwm = (255 / 4) * level;
  analogWrite(MOTOR_PIN, pwm);
}

char *recieveMessage(){
  uint8_t buf[MESSAGE_LENGTH];
  uint8_t buflen = sizeof(buf);

  if(driver.recv(buf, &buflen)){
    return (char *) buf;
  } else{
    return "";
  }
}
