//Project Libraries
#include <RH_ASK.h> //Transmitter/Reciever library
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

//Constants
#define WATER_PIN A1
#define RLED_PIN 11
#define GLED_PIN 10
#define BLED_PIN 9
#define LEVEL_0 "LEVEL_0"
#define LEVEL_1 "LEVEL_1"
#define LEVEL_2 "LEVEL_2"
#define LEVEL_3 "LEVEL_3"
#define LEVEL_4 "LEVEL_4"

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
  pinMode(OUTPUT, RLED_PIN);
  pinMode(OUTPUT, GLED_PIN);
  pinMode(OUTPUT, BLED_PIN);
  setRGB(0, 0, 0);
  delay(250);
  setRGB(255, 0, 0);
  delay(250);
  setRGB(0, 255, 0);
  delay(250);
  setRGB(0, 0, 255);
  delay(250);
  setRGB(0, 0, 0);
  delay(250);
}

void loop() {
  int input = analogRead(WATER_PIN); //can range from 0-1024
  Serial.println(input);

  //Check the ranges of the incoming input
  if (input >= 400) { //Level 3: do not send anything
    setRGB(255, 0, 0); //Red colour: WARNING!
    sendMessage(LEVEL_4);

  } else if (input >= 300) { //Level 2: do not send anything
    sendMessage(LEVEL_3);

  } else if (input >= 200) { //Level 1: do not send anything
    sendMessage(LEVEL_2);

  } else if (input >= 100) { //Level 0: do not send anything
    setRGB(255, 255, 0); //Yello colour: 
    sendMessage(LEVEL_1);

  } else if (input >= 0) { //Level 0: do not send anything
    setRGB(0, 255, 0); //Green colour: ok
    sendMessage(LEVEL_0);

  }

  delay(500);
}

void sendMessage(const char *msg) {
  Serial.print("Sent Message: ");
  Serial.println(msg);

  driver.send((uint8_t *) msg, strlen(msg));
  driver.waitPacketSent();
}

void setRGB(int r, int g, int b){  
  analogWrite(RLED_PIN, r);
  analogWrite(GLED_PIN, g);
  analogWrite(BLED_PIN, b);
}
