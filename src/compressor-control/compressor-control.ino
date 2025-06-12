// === Libraries ===
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === Constants ===
#define COMPRESSOR_START_PIN 6
#define COMPRESSOR_ON_PIN 7

#define NTC_PIN A0
#define VACUUM_SENSOR_PIN A1

// Comment-Uncomment desired or check datasheet
#define SHT30_ADDRESS 0x44   //Default wiring (ADDR = GND or floating)
//#define SHT30_ADDRESS 0x45   //Use if ADDR pin is tied to VCC

// I²C LCD + SHT30/31 will use A4 (SDA) and A5 (SCL)

// === LCD object ===
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change to 0x3F if needed


void setup() {
  // Initialize LCD
  lcd.init();          // Start LCD via I²C
  lcd.backlight();     // Turn on backlight

  lcd.setCursor(0, 0);
  lcd.print("System Booting");
  delay(10500);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:

}
