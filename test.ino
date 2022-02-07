/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "WiFi.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

int numberOfWiFiNetworks;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
      }
    }
};

void setup() {
  Serial.begin(115200);

  delay(10000);

  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());
  String s = scanForWiFis();

//  String s = String(n) + "WiFi networks found";

  pCharacteristic->setValue(s.c_str());
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  delay(2000);
}

String scanForWiFis(){
  WiFi.mode(WIFI_STA);
  // WiFi.scanNetworks will return the number of networks found
  numberOfWiFiNetworks = WiFi.scanNetworks();
  String names;
  Serial.println("scan done");
  if (numberOfWiFiNetworks == 0) {
      Serial.println("no networks found");
  } 
  for (int i = 0; i < numberOfWiFiNetworks; i++){
    Serial.println(WiFi.SSID(i));
    names += WiFi.SSID(i)+ "\n";
  }
  return names;
}

