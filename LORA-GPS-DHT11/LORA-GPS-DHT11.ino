#include <TTN_esp32.h>
#include <TTN_CayenneLPP.h>

#include <TinyGPS++.h>
#include <DHT.h>
#define DHTPIN 2 // DHT11 data pin connected to GPIO2 of ESP32
#define LDRPIN 4 // LDR data pin connected to GPIO4 of ESP32

#define DHTTYPE DHT11 // DHT 11

const char* devEui = "70B3D57ED80030B5";
const char* appEui = "0000000000000001";
const char* appKey = "80DD40E31FE3B95D4A6FEB2939207D76";

TTN_esp32 ttn;
TTN_CayenneLPP lpp;


DHT dht(DHTPIN, DHTTYPE);
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
HardwareSerial SerialGPS(2);



void message(const uint8_t* payload, size_t size, uint8_t port, int rssi) {
  Serial.println("-- MESSAGE");
  Serial.printf("Received %d bytes on port %d (RSSI=%ddB) :", size, port, rssi);
  for (int i = 0; i < size; i++) {
    Serial.printf(" %02X", payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  pinMode(LDRPIN,INPUT);

  SerialGPS.begin(GPSBaud, SERIAL_8N1, 16, 17);
  Serial.println("GPS initialization");

  dht.begin();
  Serial.println("DHT11 sensor initialized");


  ttn.begin();
  ttn.onMessage(message);  
  ttn.join(devEui, appEui, appKey);
  Serial.print("Joining TTN ");
  while (!ttn.isJoined()) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\njoined !");
  ttn.showStatus();

}

void loop() {

  while (SerialGPS.available() > 0)
  {
    char c = SerialGPS.read();
    gps.encode(c);
  }

    static unsigned long lastDisplayTime = 0;
  if (millis() - lastDisplayTime > 1000)
  {
        float humidity = dht.readHumidity();
     float temperature = dht.readTemperature();
       float longitude = 0.0;
        float latitude = 0.0;
        float altitude = 0.0;
           float speed = 0.0;
    int lightIntensity = analogRead(LDRPIN);  // Read the raw light sensor value
    int mappedIntensity = map(lightIntensity, 0, 4095, 100, 0);  // Map to a range of 100 to 0


      if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }


    lastDisplayTime = millis();


  if (ttn.sendBytes(lpp.getBuffer(), lpp.getSize())) {

              // Print GPS location
    Serial.print(F("Location: ")); 
    if (gps.location.isValid())
    {
      Serial.print(gps.location.lat(), 6);
       longitude = gps.location.lng();
        latitude = gps.location.lat();
        altitude = gps.altitude.meters();
           speed = gps.speed.kmph();  

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

    Serial.print("Light Intensity: ");
    Serial.print(mappedIntensity);
    Serial.print(" \t");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" *C");
    Serial.println();

    }

    lpp.reset();
    lpp.addTemperature(1, temperature);
    lpp.addRelativeHumidity(2, humidity);
    lpp.addGPS(1,latitude,longitude,altitude);
    lpp.addAnalogInput(1,speed);
    lpp.addAnalogInput(2,map(analogRead(LDRPIN), 0, 4095, 100, 0));



    delay(9000); 

  }



}



























