#include "WiFi.h"

char ssid[] = "yourNetwork";     //  your network SSID (name)
char pass[] = "secretPassword";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

void setup() {
  Serial.begin(115200);

  checkForWifiShield();
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to WEP network, SSID: ");
    tryToConnect();
  }
 
}

void tryToConnect(){
  // Check for available WiFis
  
  // Open BLE Connection


  // Wait for WiFi Credentials

  // Connect to WiFi
  
}

void loop() {
  // Check for Connection Status

}

boolean checkForWifiShield(){
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Your ESP32 has no shield, so I'm doing nothing");
    stopSketch();
  }
}

void stopSketch() {
  noInterrupts();
  while(true) {}
}
