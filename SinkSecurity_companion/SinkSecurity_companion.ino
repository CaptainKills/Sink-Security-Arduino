//Project Libraries
#include <RH_ASK.h> //Transmitter/Reciever library
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> //Not actually used but needed to compile
#endif

//Constants
#define MOTOR_PIN 5
#define BUF_LENGTH 6
#define LEVEL_0 "LEVEL_0"
#define LEVEL_1 "LEVEL_1"
#define LEVEL_2 "LEVEL_2"
#define LEVEL_3 "LEVEL_3"

//Variable Objects
RH_ASK driver(2000, 2, 3, 0);

void setup() {
  //Serial Debugging Setup
  Serial.begin(9600);

  //Driver initialisation
  if(!driver.init()){
    Serial.println("Driver initialisation failed!");
  }

  //PinMode initialisation
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {
  //Recieve the message
  char *message = "";
  recieveMessage(&message);
  
  Serial.print("Message: ");
  Serial.println(message);

  //Check which message has been sent
  if(strcmp(message, LEVEL_1) == 0){ //Level 1: light vibrations.`
    Serial.println("Level 1 Detected: Set Vribration Level to 1");
    setMotorLevel(1);
    
  } else if(strcmp(message, LEVEL_2) == 0){ //Level 2: medium vibrations.
    Serial.println("Level 2 Detected: Set Vribration Level to 2");
    setMotorLevel(2); 
    
  } else if(strcmp(message, LEVEL_3) == 0){ //c 3: strong vibrations.
    Serial.println("Level 3 Detected: Set Vribration Level to 3");
    setMotorLevel(3);
   
  } else if(strcmp(message, LEVEL_0) == 0){ //Level 0: disable vibrations.
    Serial.println("Level 0 Detected: Turning off.");
    setMotorLevel(0);
    
  } else{
    //Default case: no message received.
  }

  delay(100);
}

void setMotorLevel(int level){
  int pwm = 0;
  if(level != 0){
    pwm = 105 + (50 * level);
  }
  digitalWrite(MOTOR_PIN, pwm);
}

void recieveMessage(char **msg){
  uint8_t buf[BUF_LENGTH];
  uint8_t buflen = sizeof(buf);

  if(driver.recv(buf, &buflen-1)){
    buf[BUF_LENGTH] = '\0';
    *msg = (char *) buf;
  }
}
