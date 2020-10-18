//Project Libraries
#include <WiFiNINA.h> // WiFi library
#include <RH_ASK.h> // Transmitter/Reciever library
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif
#include "arduino_secrets.h" 

//Constants
#define WATER_SENSOR_PIN A7
#define INTERRUPT_PIN 2
#define RLED_PIN 11
#define GLED_PIN 10
#define BLED_PIN 9
#define LEVEL_0 "LEVEL_0"
#define LEVEL_1 "LEVEL_1"
#define LEVEL_2 "LEVEL_2"
#define LEVEL_3 "LEVEL_3"

//Variable Objects
char *current_level = LEVEL_0;
char ssid[] = SECRET_SSID; // network SSID
char pass[] = SECRET_PASS; // network password

RH_ASK driver;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  //Serial Debugging Setup
  Serial.begin(9600);

  //PinMode initialisation
  pinMode(WATER_SENSOR_PIN, INPUT);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  pinMode(RLED_PIN, OUTPUT);
  pinMode(GLED_PIN, OUTPUT);
  pinMode(BLED_PIN, OUTPUT);
  setRGB(0, 200, 0); //Set Starting colour: Green

  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  // Check if WiFi credentials are not empty:
  if(ssid.length == 0 || pass.length == 0){
    Serial.println("No WiFi Credentials Detected!");
    while (true);
  }

  // Attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    // Wait 5 seconds for connection:
    delay(5000);
  }
  
  server.begin();
  printWifiStatus();

  // Initialise Driver:
  if(!driver.init()){
    Serial.println("Driver initialisation failed!");
    while (true);
  }
}

void loop() {
  // Check for On/Off Touch sensor:
  int on_off = digitalRead(INTERRUPT_PIN);

  if(on_off == LOW){
    ArduinoSleep();
  }

  // Read the water-level sensor
  int input = analogRead(WATER_SENSOR_PIN);
  Serial.print("Input: ");
  Serial.println(input);

  //Check the ranges of the incoming input
  if (input >= 540) { //Level 3: Warning!
    setRGB(200, 0, 0); //Intense Yellow
    current_level = LEVEL_3;
    sendMessage(LEVEL_3);
  } else if (input >= 500) { //Level 2: Water Halfway
    setRGB(200, 200, 102); //Normal Yellow
    current_level = LEVEL_2;
    sendMessage(LEVEL_2);
  } else if (input >= 300) { //Level 1: Water touched Sensor
    setRGB(200, 200, 200); //Light Yello colour
    current_level = LEVEL_1;
    sendMessage(LEVEL_1);
  } else if (input >= 0) { //Level 0: No Water detected
    setRGB(0, 200, 0); //Green colour: ok
    current_level = LEVEL_0;
    sendMessage(LEVEL_0);
  }
  
  clientHandler(current_level);
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

void clientHandler(char level[]){
  // listen for incoming clients
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Client connected.");
    // An http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // Send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          
          // Output the value of each analog input pin
          client.print(level);
          break;
        }
        
        if (c == '\n') {
          // You're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // You've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // Give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("Client disconnected.");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's Local IP address:
  IPAddress ip_local = WiFi.localIP();
  Serial.print("Local IP Address: ");
  Serial.println(ip_local);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void ArduinoSleep(){
  Serial.println("Arduino going to sleep!");
  setRGB(0,0,0); //Turn off LED to show the Arduino is going to sleep
 
  while(digitalRead(INTERRUPT_PIN) == LOW){
    Serial.println("Waiting...");
    delay(2000);
  }

  Serial.println("Arduino waking up!");
  setRGB(0, 200, 0); // Turn on LED to show the Arduino is back up
}
