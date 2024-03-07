#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <WiFi.h> // To fetch the time
#define CS_PIN 5


/* PUT YOUR OWN SSID AND PASS HERE*/
const char* ssid = "";
const char* password = "";

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Initialize SD card
  if (!SD.begin(CS_PIN)) {
    Serial.println("Failed to connect 2 SD card");
    return;
  }

  // Create or overwrite a file and write a test message
  File file = SD.open("/fixaj.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to create file");
    return;
  }
  file.println("Test of writing to SD card");
  file.close();
  Serial.println("Successfully written to card");

  // Initialize NTP for Turkish Time Zone
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println("Time synchronized");
}

void loop() {
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  char formattedTime[20];
  strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M:%S", &timeInfo);
  Serial.println(formattedTime);

  // Append the current time with randomized temperature and humidity to the SD card
  File file = SD.open("/fixaj.txt", FILE_APPEND);
  if (file) {
    int randomHumidity = random(20, 81); // Generate a random humidity value between 20 and 80
    int randomTemperature = random(10, 36); // Generate a random temperature value between 10°C and 35°C
    file.print(formattedTime);
    file.printf(" - Heat: %d°C, Humidity: %d%%", randomTemperature, randomHumidity);
    file.println(); // New line
    file.close();
  } else {
    Serial.println("File could not be opened");
  }

  delay(5000); // Print time and log to SD every 5 seconds
}
