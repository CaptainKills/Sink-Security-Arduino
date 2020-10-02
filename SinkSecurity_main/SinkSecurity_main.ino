//Project Libraries
#include <RH_ASK.h> //Transmitter/Reciever library
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

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
  if (input <= 0 + INPUT_MARGIN) { //Level 0: do not send anything
    sendMessage(LEVEL_0);

  } else if (input <= 100 + INPUT_MARGIN) { //Level 0: do not send anything
    sendMessage(LEVEL_1);

  } else if (input <= 200 + INPUT_MARGIN) { //Level 1: do not send anything
    sendMessage(LEVEL_2);

  } else if (input <= 300 + INPUT_MARGIN) { //Level 2: do not send anything
    sendMessage(LEVEL_3);

  } else if (input <= 400 + INPUT_MARGIN) { //Level 3: do not send anything
    sendMessage(LEVEL_4);

  }

  delay(500);
}

void sendMessage(const char *msg) {
  Serial.print("Sent Message: ");
  Serial.println(msg);

  driver.send((uint8_t *) msg, strlen(msg));
  driver.waitPacketSent();
}
