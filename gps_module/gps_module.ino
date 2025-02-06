#include <Wire.h>          // Include the Wire library for I2C
#include <TinyGPS++.h>     // Include the TinyGPS++ library

TinyGPSPlus gps;            // Create a TinyGPS++ object

void setup() {
  Serial.begin(115200);       // Start serial communication with GPS module
  Serial.println("Waiting for GPS...");
}

void loop() {
  while (Serial.available() > 0) {
    if (gps.encode(Serial.read())) {
      if (gps.location.isValid()) {
        // Display latitude and longitude
        Serial.print("Latitude: ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(", Longitude: ");
        Serial.println(gps.location.lng(), 6);
      } else {
        Serial.println("Location: N/A");
      }

      // Display date (UTC)
      Serial.print("Date: ");
      Serial.print(gps.date.month());
      Serial.print("/");
      Serial.print(gps.date.day());
      Serial.print("/");
      Serial.println(gps.date.year());

    }
  }
}
