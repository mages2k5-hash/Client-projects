#include <Wire.h>
#include <TFT_eSPI.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000

TFT_eSPI tft = TFT_eSPI();
PulseOximeter pox;

uint32_t tsLastReport = 0;

// Callback for beat detection
void onBeatDetected() {
  Serial.println("Beat!");
}

void setup() {
  Serial.begin(115200);

  // I2C init
  Wire.begin(21, 22);

  // TFT init
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // UI Layout
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 10);
  tft.println("MAX30100 Monitor");

  tft.drawFastHLine(0, 40, 240, TFT_WHITE);

  tft.setCursor(20, 60);
  tft.println("Heart Rate:");

  tft.setCursor(20, 140);
  tft.println("SpO2:");

  // MAX30100 init
  if (!pox.begin()) {
    Serial.println("MAX30100 FAILED");
    tft.setCursor(20, 220);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("Sensor Error!");
    while (1);
  } else {
    Serial.println("MAX30100 OK");
    tft.setCursor(20, 220);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Sensor Connected");
  }

  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    float bpm = pox.getHeartRate();
    float spo2 = pox.getSpO2();

    Serial.print("BPM: ");
    Serial.print(bpm);
    Serial.print(" | SpO2: ");
    Serial.println(spo2);

    // Clear old values
    tft.fillRect(20, 90, 200, 40, TFT_BLACK);
    tft.fillRect(20, 170, 200, 40, TFT_BLACK);

    // BPM display
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(20, 90);
    if (bpm > 30 && bpm < 200)
      tft.printf("%.1f BPM", bpm);
    else
      tft.print("-- BPM");

    // SpO2 display
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(20, 170);
    if (spo2 > 50 && spo2 <= 100)
      tft.printf("%.1f %%", spo2);
    else
      tft.print("-- %");

    tsLastReport = millis();
  }
}