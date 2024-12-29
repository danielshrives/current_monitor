#include <Arduino.h>
#include <Adafruit_INA219.h>
#include <M5StickCPlus.h>

Adafruit_INA219 ina219;

int currentCalibration = 0;

void displayCalibrationSetting() {
  M5.Lcd.fillRect(180, 0, 160, 10, BLACK); // Clear the area
  M5.Lcd.setCursor(180, 0);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE);
  switch (currentCalibration) {
    case 0:
      M5.Lcd.print("16V 400mA");
      break;
    case 1:
      M5.Lcd.print("32V 1A");
      break;
    case 2:
      M5.Lcd.print("32V 2A");
      break;
  }
}

void setup()
{
  // put your setup code here, to run once:

  M5.begin();

  Serial.begin(9600);

  if (!ina219.begin())
  {
    Serial.println("Failed to find INA219 chip");
    while (1)
    {
      delay(10);
    }
  }
  // Initial calibration
  ina219.setCalibration_16V_400mA();

  M5.Lcd.setRotation(1);

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.setTextColor(WHITE);

  displayCalibrationSetting();

}

void loop()
{

  static unsigned long lastScreenUpdate = 0;
  static unsigned long lastSerialUpdate = 0;
  unsigned long currentMillis = millis();

  M5.update();

  if (M5.BtnA.wasPressed())
  {
    currentCalibration = (currentCalibration + 1) % 3;
    switch (currentCalibration)
    {
    case 0:
      ina219.setCalibration_16V_400mA();
      Serial.println("Calibration set to 16V 400mA");
      break;
    case 1:
      ina219.setCalibration_32V_1A();
      Serial.println("Calibration set to 32V 1A");
      break;
    case 2:
      ina219.setCalibration_32V_2A();
      Serial.println("Calibration set to 32V 2A");
      break;
    }
    displayCalibrationSetting();
  }

  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  if (currentMillis - lastScreenUpdate >= 500) {
    lastScreenUpdate = currentMillis;

    M5.Lcd.drawLine(0, 67, 240, 67, WHITE); // Draw a horizontal line in the middle of the screen

    // Clear previous values
    M5.Lcd.fillRect(10, 10, 150, 50, BLACK);
    M5.Lcd.fillRect(10, 77, 150, 50, BLACK);

    // Display values without units
    M5.Lcd.setTextSize(4); // Set text size to largest possible
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.printf("%.2f", loadvoltage);
    M5.Lcd.setCursor(10, 77);
    M5.Lcd.printf("%.2f", current_mA);
    // Display labels
    M5.Lcd.setCursor(160, 25);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("V");
    M5.Lcd.setCursor(160, 85);
    M5.Lcd.printf("mA");
  }

  if (currentMillis - lastSerialUpdate >= 50) {
    lastSerialUpdate = currentMillis;

    Serial.print("{");
    Serial.print("\"busvoltage\":");
    Serial.print(busvoltage);
    Serial.print(",");
    Serial.print("\"shuntvoltage\":");
    Serial.print(shuntvoltage);
    Serial.print(",");
    Serial.print("\"loadvoltage\":");
    Serial.print(loadvoltage);
    Serial.print(",");
    Serial.print("\"current_mA\":");
    Serial.print(current_mA);
    Serial.print(",");
    Serial.print("\"timestamp\":");
    Serial.print(currentMillis);
    Serial.print(",");
    Serial.print("\"power_mW\":");
    Serial.print(power_mW);
    Serial.println("}");
  }
}
