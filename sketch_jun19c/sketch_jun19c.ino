#include <TinyGPS++.h>

// Define GPS baud rate
static const uint32_t GPSBaud = 9600;

// Create a TinyGPSPlus object
TinyGPSPlus gps;

// Initialize Serial2 for the GPS module (TX: 17, RX: 16)
HardwareSerial SerialGPS(2);

void setup()
{
  // Initialize Serial monitor communication
  Serial.begin(9600);

  // Initialize Serial2 (pins 17 as TX and 16 as RX) for GPS communication
  SerialGPS.begin(GPSBaud, SERIAL_8N1, 16, 17);

  Serial.println("GPS initialization");
}

void loop()
{
  // Check if data is available from GPS module
  while (SerialGPS.available() > 0)
  {
    // Read a character from the GPS module
    char c = SerialGPS.read();
  
    
    // Encode the character to the TinyGPSPlus library
    gps.encode(c);
  }

  // Display the parsed GPS information every second
  static unsigned long lastDisplayTime = 0;
  if (millis() - lastDisplayTime > 1000)
  {
    displayInfo();
    lastDisplayTime = millis();
  }

  // Check if no GPS data is processed after 30 seconds
  if (millis() > 30000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring and ensure the GPS module has a clear view of the sky."));
    while(true);
  }
}

void displayInfo()
{
  // Print GPS location
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  // Print GPS date
  Serial.print(F("  Date "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  // Print GPS time
  Serial.print(F("  Time "));
  if (gps.time.isValid())
  {
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    Serial.print(gps.time.second());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
