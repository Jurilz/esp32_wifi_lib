/*

  ESP32WifiConfigurator.cpp - Library for configuring the wifi credentials 
  of a esp32 controller via BLE.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: GNU General Public License v3 (GPL-3).
*/


#include "Arduino.h"
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEServer.h"
#include "BLE2902.h"
#include "WiFi.h"
#include "ESP32WifiConfigurator.h"
#include "GeneralCommunicationCallback.h"
#include "WifiConfiguratorCallback.h"

#define _SERVICE_UUID                                  "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define _AVAILABE_WIFI_NETWORKS_CHARACTERISTIC_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define _WIFI_SETUP_CHARACTERISTIC_UUID                "59a3861e-8d11-4f40-9597-912f562e4759"

// #define _NEW_LINE_SEPERATOR "\r\n"
// #define _CLOSED   "CLOSED"
// #define _SUCCESS  "SUCCESS"

#define _WAIT_TIME 5000
#define LED_BUILTIN 2


char* _deviceName = "myCoolESP32Device";


BLECharacteristic* ESP32WifiConfigurator::_availableWifiNetworks = nullptr;
BLECharacteristic* _wifiConfiguration = nullptr;
boolean ESP32WifiConfigurator::_bleServerStarted = false;

ESP32WifiConfigurator::ESP32WifiConfigurator() {}

/**
 * @brief Return the characteristic by handle.
 * @return The characteristic.
 */
void ESP32WifiConfigurator::startWifiConfigurator() {
    WiFi.mode(WIFI_STA);
    startBLE();
}


boolean ESP32WifiConfigurator::connectToWiFi(const char ssid[], const char pw[]) {
  WiFi.begin(ssid, pw);
  delay(_WAIT_TIME);
  if (WiFi.status() == WL_CONNECTED) {
      digitalWrite(LED_BUILTIN, HIGH);
      return true;
  }
  return false;
}


void ESP32WifiConfigurator::setUpBLECharacteristics(BLEService* wifiConfigureService){
    _availableWifiNetworks = wifiConfigureService->createCharacteristic(
        _AVAILABE_WIFI_NETWORKS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE| 
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE);

    _wifiConfiguration = wifiConfigureService->createCharacteristic(
                            _WIFI_SETUP_CHARACTERISTIC_UUID,
                            BLECharacteristic::PROPERTY_WRITE);

    _availableWifiNetworks->setCallbacks(new GeneralCommunicationCallback());
    _availableWifiNetworks->addDescriptor(new BLE2902());
    
    _wifiConfiguration->setCallbacks(new WifiConfigurationCallback());
    _wifiConfiguration->addDescriptor(new BLE2902());
    String scannedNetworkNames = scanForWiFis();

    _availableWifiNetworks->setValue(scannedNetworkNames.c_str());
}

void ESP32WifiConfigurator::startBLE(){
  if (_bleServerStarted) return;
  
  BLEDevice::init("TESDT");
  BLEServer *wifiConfigureServer = BLEDevice::createServer();

  _bleServerStarted = true;
  
  BLEService* wifiConfigureService = wifiConfigureServer->createService(_SERVICE_UUID);

  setUpBLECharacteristics(wifiConfigureService);

  wifiConfigureService->start();

  BLEAdvertising* pAdvertising = wifiConfigureServer->getAdvertising();
  pAdvertising->addServiceUUID(_SERVICE_UUID);
  pAdvertising->start();

  /// changed
  String scannedNetworkNames = scanForWiFis();
  _availableWifiNetworks->setValue(scannedNetworkNames.c_str());
}

void ESP32WifiConfigurator::stopBLE(){
  if (!_bleServerStarted) return;
    BLEDevice::deinit(true);
    _bleServerStarted = false;
}

String ESP32WifiConfigurator::scanForWiFis() {
  int numberOfWiFiNetworks = WiFi.scanNetworks();
  String names;
  for (int i = 0; i < numberOfWiFiNetworks; i++) {
      names += WiFi.SSID(i) + "\n";
  }
  return names;
}






