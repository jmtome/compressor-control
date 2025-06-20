// === Libraries ===
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === Constants ===
#define COMPRESSOR_START_PIN 6
#define COMPRESSOR_ON_PIN 7

#define NTC_PIN A0
#define VACUUM_SENSOR_PIN A1

// Comment-Uncomment desired or check datasheet
#define SHT30_ADDRESS 0x44  //Default wiring (ADDR = GND or floating)
//#define SHT30_ADDRESS 0x45   //Use if ADDR pin is tied to VCC

// I²C LCD + SHT30/31 will use A4 (SDA) and A5 (SCL)

// === LCD object ===
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change to 0x3F if needed


// === Compressor Control Configuration ===
#define TEMP_THRESHOLD 35.0     // °C - NTC trigger point
#define VACUUM_THRESHOLD 20.0   // kPa - Vacuum trigger point
#define START_DURATION_MS 6000  // ms - Time START stays on before switching to RUN
#define MIN_RUN_TIME_MS 10000   // ms - Minimum ON duration before shutdown allowed

// Enum for Compressor State Machine
enum CompressorState { OFF,
                       STARTING,
                       RUNNING };
CompressorState compressorState = OFF;

unsigned long compressorStartTime = 0;
unsigned long compressorRunStartTime = 0;


void setup() {
  // Initialize LCD
  Serial.begin(9600);
  lcd.init();       // Start LCD via I²C
  lcd.backlight();  // Turn on backlight

  lcd.setCursor(0, 0);
  lcd.print("System Booting");
  delay(1500);
  lcd.clear();
  lcd.print("System Loaded");
  lcd.setCursor(0, 1);
  lcd.print("Welcome Sir.");
  delay(1500);
  lcd.clear();
  // Compressor outputs
  pinMode(COMPRESSOR_START_PIN, OUTPUT);
  pinMode(COMPRESSOR_ON_PIN, OUTPUT);

  digitalWrite(COMPRESSOR_START_PIN, LOW);
  digitalWrite(COMPRESSOR_ON_PIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  // === Read analog sensors ===
  int ntcRaw = analogRead(NTC_PIN);               // From A0
  int vacuumRaw = analogRead(VACUUM_SENSOR_PIN);  // From A1
  // Mocked humidity for now
  float humidity = 55.0;

  // Convert raw values (0–1023) to meaningful units (mocked)
  float temperatureC = map(ntcRaw, 0, 1023, -10, 60);   // Example range: -10°C to 60°C
  float vacuumLevel = map(vacuumRaw, 0, 1023, 0, 100);  // Example range: 0–100 kPa

  // OPTIONAL: print to serial for debugging
  Serial.print("Temp: ");
  Serial.print(temperatureC);
  Serial.print(" °C, Vacuum: ");
  Serial.print(vacuumLevel);
  Serial.println(" kPa");

  // Add delay if needed
  delay(1000);

  displaySensorData(temperatureC, humidity, vacuumLevel);
  updateCompressorControl(temperatureC, vacuumLevel);
}

void displaySensorData(float tempC, float humidity, float vacuum) {
  // === First row ===
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(tempC, 1);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.print("% ");

  // Countdown or '-'
  switch (compressorState) {
    case STARTING:
      {
        int countdown = (START_DURATION_MS - (millis() - compressorStartTime)) / 1000;
        if (countdown <= 0) {
          lcd.print("--");
        } else {
          if (countdown < 10) lcd.print("0");
          lcd.print(countdown);
        }
        break;
      }
    case RUNNING:
    case OFF:
      lcd.print("--");
      break;
  }

  // === Second row ===
  lcd.setCursor(0, 1);
  lcd.print("Vac: ");

  // Pad vacuum if <10
  if (vacuum < 10) lcd.print(" ");
  lcd.print((int)vacuum);
  lcd.print("kPa.  ");

  // Compressor state in last 3 chars
  switch (compressorState) {
    case STARTING:
      lcd.print("STA");
      break;
    case RUNNING:
      lcd.print("RUN");
      break;
    case OFF:
      lcd.print("STO");
      break;
  }
}


void updateCompressorControl(float temperatureC, float vacuumLevel) {
  bool shouldRun = (temperatureC > TEMP_THRESHOLD && vacuumLevel < VACUUM_THRESHOLD);
  unsigned long now = millis();

  switch (compressorState) {
    case OFF:
      if (shouldRun) {
        Serial.println("Compressor STARTING");
        digitalWrite(COMPRESSOR_START_PIN, HIGH);
        digitalWrite(COMPRESSOR_ON_PIN, HIGH);
        compressorStartTime = now;
        compressorRunStartTime = now;
        compressorState = STARTING;
      }
      break;

    case STARTING:
      if (now - compressorStartTime >= START_DURATION_MS) {
        Serial.println("Compressor RUNNING");
        digitalWrite(COMPRESSOR_START_PIN, LOW);  // Turn off START, keep ON
        compressorState = RUNNING;
      }
      break;

    case RUNNING:
      if (!shouldRun && (now - compressorRunStartTime >= MIN_RUN_TIME_MS)) {
        Serial.println("Compressor STOPPED");
        digitalWrite(COMPRESSOR_START_PIN, LOW);
        digitalWrite(COMPRESSOR_ON_PIN, LOW);
        compressorState = OFF;
      }
      break;
  }
}
