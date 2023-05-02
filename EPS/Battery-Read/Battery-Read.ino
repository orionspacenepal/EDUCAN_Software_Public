#define batPin A0
void setup() {
   Serial.begin(9600);
}

void loop() {
  
  int rawValue = analogRead(batPin);
  float voltage=2*map(rawValue, 0, 1023, 0, 4.2);
  Serial.print("Voltage of battery= ");
  Serial.println(voltage);
  delay(1000);        
}
