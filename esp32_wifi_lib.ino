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

#define LED_BUILTIN 2

// BLE UUIDs
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define AVAILABE_WIFI_NETWORKS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CONNECTION_STATUS_CHARACTERISTIC_UUID "59a3861e-8d11-4f40-9597-912f562e4759"

#define BAUD_RATE 115200
#define TIME_INTERVAL 3000
#define WAIT_TIME 5000

BLECharacteristic *availableWifiNetworks;
BLECharacteristic *wifiConfiguration;
boolean bleServerStarted = false;

void setUpBLECharacteristics(BLEService* wifiConfigureService);

void connectToWiFi(const char ssid[], const char pw[]) {
  WiFi.begin(ssid, pw);
  delay(WAIT_TIME);
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

class WifiConfigurationCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string const value = pCharacteristic->getValue();

      char* p = strdup(value.c_str());
      if (value.length() > 0) {
        for (int i = 0; i < value.length(); i++) {
          Serial.print(value[i]);
        }
        Serial.println();

        char* ssid = strtok(p,"\r\n");
        char* pw = strtok(NULL,"\r\n");
        
        connectToWiFi(ssid, pw);
      }
      free(p);
    }
};

void setup() {
  Serial.begin(BAUD_RATE);

  WiFi.mode(WIFI_STA);

  // Pin mode for LED Pin
  pinMode(LED_BUILTIN, OUTPUT);
}

void setUpBLECharacteristics(BLEService* wifiConfigureService){
    availableWifiNetworks = wifiConfigureService->createCharacteristic(
          AVAILABE_WIFI_NETWORKS_CHARACTERISTIC_UUID,
          BLECharacteristic::PROPERTY_READ);
    wifiConfiguration = wifiConfigureService->createCharacteristic(
                             CONNECTION_STATUS_CHARACTERISTIC_UUID,
                             BLECharacteristic::PROPERTY_WRITE);
    
    wifiConfiguration->setCallbacks(new WifiConfigurationCallback());
    String scannedNetworkNames = scanForWiFis();
  
    availableWifiNetworks->setValue(scannedNetworkNames.c_str());
}

void startBLE(){
  if (bleServerStarted) return;
  
  BLEDevice::init("MyESP32");
  BLEServer *wifiConfigureServer = BLEDevice::createServer();

  bleServerStarted = true;
  
  BLEService* wifiConfigureService = wifiConfigureServer->createService(SERVICE_UUID);

  setUpBLECharacteristics(wifiConfigureService);

  wifiConfigureService->start();

  BLEAdvertising* pAdvertising = wifiConfigureServer->getAdvertising();
  pAdvertising->start();
}

void updateAvailableWifiNetworks(){
  if (!bleServerStarted) return;
  String scannedNetworkNames = scanForWiFis();
  availableWifiNetworks->setValue(scannedNetworkNames.c_str());
}

void stopBLE(){
  if (!bleServerStarted) return;
  BLEDevice::deinit(true);
  bleServerStarted = false;
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to wifi");
    IPAddress myAddress = WiFi.localIP();
    Serial.print("RRSI: ");
    Serial.println(WiFi.RSSI());
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    stopBLE();
  } else {
    startBLE();
    updateAvailableWifiNetworks();
  }
  delay(TIME_INTERVAL);
}

String scanForWiFis() {
  // WiFi.scanNetworks will return the number of networks found
  int numberOfWiFiNetworks = WiFi.scanNetworks();
  String names;
  Serial.println("scan done");
  if (numberOfWiFiNetworks == 0) {
    Serial.println("No networks found");
    return names;
  }
  for (int i = 0; i < numberOfWiFiNetworks; i++) {
    Serial.println(WiFi.SSID(i));
    names += WiFi.SSID(i) + "\n";
  }
  return names;
}
