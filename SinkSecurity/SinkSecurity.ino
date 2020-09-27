#define WATER_PIN A1

void setup() {
  Serial.begin(9600);
  pinMode(INPUT, WATER_PIN);

}

void loop() {
  int input = analogRead(WATER_PIN);
  Serial.println(input);

}
