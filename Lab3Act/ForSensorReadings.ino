#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define MPU_ADDR 0x68   // change to 0x69 if scanner shows that

LiquidCrystal_I2C lcd(0x27, 16, 2); // change to 0x3F if needed

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  lcd.init();
  lcd.backlight();

  Serial.println("Multi-Sensor Data Collection");
  Serial.println("-----------------------------");

  // Wake MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  if (Wire.endTransmission(true) != 0) {
    Serial.println("MPU6050 NOT detected!");
    lcd.setCursor(0,0);
    lcd.print("MPU ERROR");
    while(1);
  }

  lcd.setCursor(0,0);
  lcd.print("MPU OK");
  delay(1000);
  lcd.clear();
}

void loop() {

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  if (Wire.endTransmission(false) != 0) {
    Serial.println("I2C Read Failed");
    return;
  }

  Wire.requestFrom(MPU_ADDR, 14, true);

  if (Wire.available() == 14) {

    int16_t AcX = Wire.read() << 8 | Wire.read();
    int16_t AcY = Wire.read() << 8 | Wire.read();
    int16_t AcZ = Wire.read() << 8 | Wire.read();
    int16_t TempRaw = Wire.read() << 8 | Wire.read();
    int16_t GyX = Wire.read() << 8 | Wire.read();
    int16_t GyY = Wire.read() << 8 | Wire.read();
    int16_t GyZ = Wire.read() << 8 | Wire.read();

    float ax = AcX / 16384.0;
    float ay = AcY / 16384.0;
    float az = AcZ / 16384.0;
    float temperature = (TempRaw / 340.0) + 36.53;

    // Serial Output (FOR SUBMISSION)
    Serial.print("Accel: ");
    Serial.print(ax); Serial.print(", ");
    Serial.print(ay); Serial.print(", ");
    Serial.print(az);
    Serial.print(" | Temp: ");
    Serial.println(temperature);

    // LCD Display
    lcd.setCursor(0,0);
    lcd.print("AX:");
    lcd.print(ax,1);
    lcd.print(" AY:");
    lcd.print(ay,1);
    lcd.print("   ");

    lcd.setCursor(0,1);
    lcd.print("T:");
    lcd.print(temperature,1);
    lcd.print("C     ");

  } else {
    Serial.println("Data not received");
  }

  delay(1000);
}