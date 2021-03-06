/*

  ESP32WifiConfigurator.cpp - Library for configuring the wifi credentials 
  of a esp32 controller via BLE.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: Apache License 2.0 (Apache-2.0).
*/


#include "Arduino.h"
#include "BLEDevice.h"
#include "BLE2902.h"
#include "WiFi.h"
#include "ESP32WifiConfigurator.h"
#include "GeneralCommunicationCallback.h"
#include "WifiConfiguratorCallback.h"

#define _SERVICE_UUID                                  "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define _AVAILABE_WIFI_NETWORKS_CHARACTERISTIC_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define _WIFI_SETUP_CHARACTERISTIC_UUID                "59a3861e-8d11-4f40-9597-912f562e4759"

#define _CLOSED   "CLOSED"
#define _SUCCESS  "SUCCESS"

#define _OPEN_WIFI            "0"
#define _CLOSED_WIFI          "1"
#define _NEW_LINE_SEPERATOR   "\n"

#define _WAIT_TIME 5000
//TODO: for debug


BLECharacteristic* ESP32WifiConfigurator::_availableWifiNetworks = nullptr;
BLECharacteristic* _wifiConfiguration = nullptr;

ESP32WifiConfigurator::ESP32WifiConfigurator(char deviceName[]) {
  _deviceName = deviceName;
}

/**
 * @brief Checks for WiFi connection and starts the BLE Server if no connection is present.
 */
void ESP32WifiConfigurator::startWifiConfigurator() {
    WiFi.mode(WIFI_STA);
    if (WiFi.status() != WL_CONNECTED) {
      startBLE();
    }
}

/**
 * @brief Disconnects from current WiFi connection and starts the BLE Server.
 */
void ESP32WifiConfigurator::restartWifiConfigurator() {
    WiFi.disconnect();
    if (WiFi.getMode() != WIFI_STA) {
       WiFi.mode(WIFI_STA);
    }
    startBLE();
}


/**
 * @brief Connects to the given wifi network.
 * @param ssid  the name of wifi network to connect to
 * @param pw    the credentials for the wifi network to connect to
 */
boolean ESP32WifiConfigurator::connectToWiFi(const char ssid[], const char pw[]) {
  if (String(pw).length() == 0) {
    WiFi.begin(ssid);
  } else {
    WiFi.begin(ssid, pw);
  }

  delay(_WAIT_TIME);
  if (WiFi.status() == WL_CONNECTED) {
      return true;
  }
  return false;
}

/**
 * @brief Setup for BLE Service and the two BLE Characteristics _availableWifiNetworks 
 * and _wifiConfiguration.
 */
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

    _availableWifiNetworks->setCallbacks(new GeneralCommunicationCallback(ESP32WifiConfigurator::connectionClosedCallback));
    _availableWifiNetworks->addDescriptor(new BLE2902());
    
    _wifiConfiguration->setCallbacks(new WifiConfiguratorCallback(ESP32WifiConfigurator::wifiSuccessCallback));
    _wifiConfiguration->addDescriptor(new BLE2902());
}

/**
 * @brief Starts the BLE Server with the device name given in the constructor.
 */
void ESP32WifiConfigurator::startBLE(){
  
  BLEDevice::init(_deviceName);
  BLEServer *wifiConfigureServer = BLEDevice::createServer();
  
  BLEService* wifiConfigureService = wifiConfigureServer->createService(_SERVICE_UUID);

  setUpBLECharacteristics(wifiConfigureService);

  wifiConfigureService->start();

  BLEAdvertising* pAdvertising = wifiConfigureServer->getAdvertising();
  pAdvertising->addServiceUUID(_SERVICE_UUID);
  pAdvertising->start();

  String scannedNetworkNames = scanForWiFis();
  _availableWifiNetworks->setValue(scannedNetworkNames.c_str());
}


/**
 * @brief Scans for avaliable wifi networks and returns their names.
 * @return one String containing the scanned WiFi names seperated by newline
 */
String ESP32WifiConfigurator::scanForWiFis() {
  int numberOfWiFiNetworks = WiFi.scanNetworks();
  String names;
  for (int i = 0; i < numberOfWiFiNetworks; i++) {
      String newWiFi = WiFi.SSID(i);
      
      if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {
        newWiFi += _OPEN_WIFI;
      } else {
        newWiFi += _CLOSED_WIFI;
      }
      newWiFi += _NEW_LINE_SEPERATOR;

    //ESP_GATT_MAX_ATTR_LEN is 600 Bytes
    if (sizeof(names + newWiFi) < 600){
      names += newWiFi;
    }
  }
  return names;
}

/**
 * @brief Connects to the given wifi network and sets a success notification.
 * @param ssid  the name of wifi network to connect to
 * @param pw    the credentials for the wifi network to connect to
 */
void ESP32WifiConfigurator::wifiSuccessCallback(const char ssid[], const char pw[]) {
      if (ESP32WifiConfigurator::connectToWiFi(ssid, pw)) {
        ESP32WifiConfigurator::_availableWifiNetworks->setValue(_SUCCESS);
        ESP32WifiConfigurator::_availableWifiNetworks->notify();
    }
}

/**
 * @brief Checks if the mobily App closed the connection and creats a RTOS task to shut down the BLE Server.
 * @param message  containing the App message
 */
void ESP32WifiConfigurator::connectionClosedCallback(const char message[]) {
      if (String(message).equals(_CLOSED)) {
        // shuts down BLE Server
        
         xTaskCreate(
            stopBLE,    
            "Stop BLE",
            1000,
            NULL,
            1,
            NULL
        );
    }
}

/**
 * @brief RTOS task to shut down the BLE Server without releasing the memory.
 */
void ESP32WifiConfigurator::stopBLE(void* parameter) {
   BLEDevice::deinit(false);
   vTaskDelete(NULL);
}
