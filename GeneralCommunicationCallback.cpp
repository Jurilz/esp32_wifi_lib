/*
  GeneralCommunicationCallback.h - BLECharacteristicCallbacks for ESP32WifiConfigurator library.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: GNU General Public License v3 (GPL-3).
*/

#include "Arduino.h"
#include "BLECharacteristic.h"
#include "GeneralCommunicationCallback.h"


GeneralCommunicationCallback::GeneralCommunicationCallback(void (*pCallbackFunc)(const char message[])) {
  _pCallbackFunc = pCallbackFunc;
}

void GeneralCommunicationCallback::onWrite(BLECharacteristic *pCharacteristic){
  std::string const value = pCharacteristic->getValue();

  char* message = strdup(value.c_str());
  if (value.length() > 0) {
    // TODO: remove debug printing
    for (int i = 0; i < value.length(); i++) {
      Serial.print(value[i]);
    }
    Serial.println();
    // 
    _pCallbackFunc(message);
  }
  free(message);
}