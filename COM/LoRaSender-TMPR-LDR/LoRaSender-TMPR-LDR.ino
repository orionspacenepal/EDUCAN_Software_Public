#include <SPI.h>
#include <LoRa.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 5
#define LDR A1

OneWire oneWire(ONE_WIRE_BUS);

int counter = 0;

DallasTemperature TemprSensor(&oneWire);

void setup() {
  Serial.begin(9600);
  pinMode(LDR, INPUT);
  TemprSensor.begin();
  while (!Serial);
  LoRa.setPins(8, 9, 2);

  Serial.println("EDUCAN COM TEST");
  TemprSensor.requestTemperatures();

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  int ldrvalue = analogRead(LDR);
  Serial.println(counter);
  float tempC = TemprSensor.getTempCByIndex(0);
  if (tempC != DEVICE_DISCONNECTED_C)
  {
    //Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);
  }
  else
  {
    Serial.println("Error: Could not read temperature data");
  }
  // send packet
  LoRa.beginPacket();
  //LoRa.print("hello ");
  Serial.print("Sending data via LoRa: ");
  Serial.print(tempC);
  Serial.print(",");
  Serial.println(ldrvalue);


  LoRa.print(tempC);
  LoRa.print(",");
  LoRa.print(ldrvalue);
  LoRa.endPacket();

  counter++;

  delay(2000);
}
