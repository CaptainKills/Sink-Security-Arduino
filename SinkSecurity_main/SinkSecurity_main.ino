//Project Libraries
#include <RH_ASK.h> //Transmitter/Reciever library
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> //Not actually used but needed to compile
#endif

//Constants
#define WATER_POWER_PIN 2
#define WATER_SENSOR_PIN A1
#define RLED_PIN 11
#define GLED_PIN 10
#define BLED_PIN 9
#define LEVEL_0 "LEVEL_0"
#define LEVEL_1 "LEVEL_1"
#define LEVEL_2 "LEVEL_2"
#define LEVEL_3 "LEVEL_3"

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
  pinMode(OUTPUT, WATER_POWER_PIN);
  pinMode(INPUT, WATER_SENSOR_PIN);
  pinMode(OUTPUT, RLED_PIN);
  pinMode(OUTPUT, GLED_PIN);
  pinMode(OUTPUT, BLED_PIN);

  digitalWrite(WATER_POWER_PIN, LOW);
  setRGB(0, 200, 0); //Set Starting colour: Green
}

void loop() {
  int input = readWaterSensor(); //can range from 0-1024
  Serial.print("Input: ");
  Serial.println(input);

  //Check the ranges of the incoming input
  if (input >= 540) { //Level 3: Warning!
    setRGB(200, 0, 0); //Intense Yellow
    sendMessage(LEVEL_3);
  } else if (input >= 500) { //Level 2: Water Halfway
    setRGB(200, 200, 102); //Normal Yellow
    sendMessage(LEVEL_2);
  } else if (input >= 300) { //Level 1: Water touched Sensor
    setRGB(200, 200, 200); //Light Yello colour
    sendMessage(LEVEL_1);

  } else if (input >= 0) { //Level 0: No Water detected
    setRGB(0, 255, 0); //Green colour: ok
    sendMessage(LEVEL_0);
  }
  
  delay(500);
}

int readWaterSensor(){
  digitalWrite(WATER_POWER_PIN, HIGH);
  delay(10);
  int sensor_value = analogRead(WATER_SENSOR_PIN);
  digitalWrite(WATER_POWER_PIN, LOW);

  return sensor_value;
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
