#include <TTN_esp32.h>
#include <TTN_CayenneLPP.h>

const char* devEui = "70B3D57ED80030B5";
const char* appEui = "0000000000000001";
const char* appKey = "80DD40E31FE3B95D4A6FEB2939207D76";

TTN_esp32 ttn;
TTN_CayenneLPP lpp;

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
  float temperature=random(-1,38);
  float humidity=random(0,100);
  float z=random(0,40);


  lpp.reset();
  lpp.addTemperature(1, temperature);
  lpp.addRelativeHumidity(2, humidity);
  lpp.addAnalogInput(1,z);

  if (ttn.sendBytes(lpp.getBuffer(), lpp.getSize())) {
    Serial.println(" ");    
    Serial.println("Sent Data To TTN:");
                   Serial.print("temperature: ");
    Serial.println(temperature);
    Serial.print("humidity: ");
    Serial.println(humidity);
    Serial.print("Z: ");
    Serial.println(z);
            
  }
  delay(10000); 


}



























