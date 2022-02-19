/*
  WifiConfiguratorCallback.h - BLECharacteristicCallbacks for ESP32WifiConfigurator library.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: GNU General Public License v3 (GPL-3).
*/

#include "Arduino.h"
#include "BLECharacteristic.h"
#include "WifiConfiguratorCallback.h"

#define _NEW_LINE_SEPERATOR "\r\n"

WifiConfiguratorCallback::WifiConfiguratorCallback(void (*pCallbackFunc)(const char ssid[], const char pw[])) {
  _pCallbackFunc = pCallbackFunc;
}

void WifiConfiguratorCallback::onWrite(BLECharacteristic *pCharacteristic){
  std::string const value = pCharacteristic->getValue();

  char* message = strdup(value.c_str());
  if (value.length() > 0) {

    char* ssid = strtok(message, _NEW_LINE_SEPERATOR);
    char* pw = strtok(NULL, _NEW_LINE_SEPERATOR);
    _pCallbackFunc(ssid, pw);
  }
  free(message);
}


