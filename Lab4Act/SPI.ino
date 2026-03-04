#include <Wire.h>

// Define your I2C pins for ESP32
#define I2C_SDA 21
#define I2C_SCL 22

void setup() {
  // Pass the defined pins to the Wire library
  Wire.begin(I2C_SDA, I2C_SCL);
  
  Serial.begin(115200); // Higher baud rate is standard for ESP32
  while (!Serial); 
  Serial.println("\nI2C Scanner (ESP32) Initialized");
  Serial.print("SDA Pin: "); Serial.println(I2C_SDA);
  Serial.print("SCL Pin: "); Serial.println(I2C_SCL);
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    } 
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    Serial.println("No I2C devices found. Check wiring and power!\n");
  } else {
    Serial.println("Scan complete.\n");
  }

  delay(5000); 
}