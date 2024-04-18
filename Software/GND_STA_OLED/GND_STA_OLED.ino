#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <LoRa.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int maxPacketSize = 150; // Maximum expected packet size, adjust as needed

void setup() {
  Serial.begin(9600);
//  while (!Serial);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(100);

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(10, 10);
  // Display static text
  display.println("GROUND\n\nSTATION");
  display.display(); 
  
  delay(100);

  LoRa.setPins(8, 9, 2);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("LoRa Receiver");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  display.clearDisplay();
  display.setTextSize(1);
  if (packetSize) {
    Serial.print("Received Data From Satellite:\n");
    
    char receivedData[maxPacketSize];
    int bytesRead = LoRa.readBytes(receivedData, maxPacketSize);
    receivedData[bytesRead] = '\0';
    
    display.setCursor(5, 10);
    display.println("Receiving Data:");
   delay(1000);
   display.clearDisplay();
    display.setCursor(0, 5);
    display.println(receivedData);
    display.display();

    Serial.println(receivedData);
  }
  else
  {
    Serial.println("No data");
  }

}
