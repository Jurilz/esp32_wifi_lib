/*

  ESP32WifiConfigurator.cpp - Library for configuring the wifi credentials 
  of a esp32 controller via BLE.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: GNU General Public License v3 (GPL-3).
*/

#include "BLECharacteristic.h"
#include "ESP32WifiConfigurator.h"
#include "WifiConfiguratorCallback.h"

#define _NEW_LINE_SEPERATOR "\r\n"
#define _SUCCESS  "SUCCESS"

// give callback method or characteristics nad connect method
WifiConfigurationCallback::WifiConfigurationCallback() {}

void WifiConfigurationCallback::onWrite(BLECharacteristic *pCharacteristic){
  std::string const value = pCharacteristic->getValue();

  char* message = strdup(value.c_str());
  if (value.length() > 0) {

    char* ssid = strtok(message, _NEW_LINE_SEPERATOR);
    char* pw = strtok(NULL, _NEW_LINE_SEPERATOR);
    
    if (ESP32WifiConfigurator::connectToWiFi(ssid, pw)) {
      ESP32WifiConfigurator::_availableWifiNetworks->setValue(_SUCCESS);
      ESP32WifiConfigurator::_availableWifiNetworks->notify();
    }
  }
  free(message);
}


