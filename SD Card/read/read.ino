#include "FS.h"
#include "SD.h"
#include "SPI.h"
#define CS_PIN 5

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(200); // Wait for the serial port to connect

  if (!SD.begin(CS_PIN)) {
    Serial.println("SD card could not be utilized");
    return;
  }

  File file = SD.open("/fixaj.txt");
  if (!file) {
    Serial.println("Could not read");
    return;
  }

  Serial.println("The record:");
  while (file.available()) {
    String line = file.readStringUntil('\n');
    Serial.println(line);
  }

  file.close();
}

void loop() {
  // Do nothing here.
}
