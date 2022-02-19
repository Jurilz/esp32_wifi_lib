
#include "ESP32WifiConfigurator.h"
#include "WiFi.h"

#define BAUD_RATE 115200
#define LED_BUILTIN 2

ESP32WifiConfigurator wifiConfigurator("BLE-Server");

void setup()
{
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);
  wifiConfigurator.startWifiConfigurator();
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected to wifi");
        IPAddress myAddress = WiFi.localIP();
        Serial.print("RRSI: ");
        Serial.println(WiFi.RSSI());
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());
  }
  delay(3000);
    
}
