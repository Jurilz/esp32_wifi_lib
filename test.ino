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
#define AVAILABE_WIFI_NETWORKS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CONNECTION_STATUS_CHARACTERISTIC_UUID "59a3861e-8d11-4f40-9597-912f562e4759"

#define LED_BUILTIN 2

int numberOfWiFiNetworks;
char ssid[] = "myhomeismyKassel";
// needs third charactersistics
BLECharacteristic *wifiConnectionStatus;
BLECharacteristic *availableWifiNetworks;



class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string const value = pCharacteristic->getValue();

      const char* p = value.c_str();

      if (value.length() > 0) {
        for (int i = 0; i < value.length(); i++) {
          Serial.print(value[i]);
        }
        Serial.println();
        connectToWiFi(p);
      }
    }
};

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  // Pin mode for LED Pin
  pinMode(LED_BUILTIN, OUTPUT);

  setupBLEServer();
}

void setupBLEServer(){
  // BLE geraffel nur wenn keine wifi connection
  BLEDevice::init("MyESP32");
  BLEServer *wifiConfigureServer = BLEDevice::createServer();

  BLEService *wifiConfigureService = wifiConfigureServer->createService(SERVICE_UUID);

  availableWifiNetworks = wifiConfigureService->createCharacteristic(
        AVAILABE_WIFI_NETWORKS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
      );
  wifiConnectionStatus = wifiConfigureService->createCharacteristic(
                           CONNECTION_STATUS_CHARACTERISTIC_UUID,
                           BLECharacteristic::PROPERTY_READ
                         );
  updateConnectionStatus();
  
  availableWifiNetworks->setCallbacks(new MyCallbacks());
  String s = scanForWiFis();

  // depens upon connection status
  availableWifiNetworks->setValue(s.c_str());
  wifiConfigureService->start();

  BLEAdvertising *pAdvertising = wifiConfigureServer->getAdvertising();
  pAdvertising->start();
}

void tearDownBLEServer(){
  //BLEDevice::deinit(true);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    tearDownBLEServer();
    printSomeDebuvStuff();
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    setupBLEServer();
  }
  updateConnectionStatus();
  delay(3000);
}

void updateConnectionStatus() {
  int&& wifiStatus = WiFi.status();
  wifiConnectionStatus->setValue(wifiStatus);
  wifiConnectionStatus->notify();
}

void printSomeDebuvStuff(){
    Serial.println("Connected to wifi");
    IPAddress myAddress = WiFi.localIP();
    Serial.print("RRSI: ");
    Serial.println(WiFi.RSSI());
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
}

void sendAvailableWifiNetworks(){
  String s = scanForWiFis();
  // depens upon connection status
  availableWifiNetworks->setValue(s.c_str());
  availableWifiNetworks->notify();
}

void connectToWiFi(const char pw[]) {
  WiFi.begin(ssid, pw);
  delay(10000);
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

String scanForWiFis() {
  // WiFi.scanNetworks will return the number of networks found
  numberOfWiFiNetworks = WiFi.scanNetworks();
  String names;
  Serial.println("scan done");
  if (numberOfWiFiNetworks == 0) {
    Serial.println("No networks found");
    names = "No networks found";
  }
  for (int i = 0; i < numberOfWiFiNetworks; i++) {
    Serial.println(WiFi.SSID(i));
    names += WiFi.SSID(i) + "\n";
  }
  return names;
}
