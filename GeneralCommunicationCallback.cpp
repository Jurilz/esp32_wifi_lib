/*
  GeneralCommunicationCallback.h - BLECharacteristicCallbacks for ESP32WifiConfigurator library.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: Apache License 2.0 (Apache-2.0).
*/

#include "Arduino.h"
#include "BLECharacteristic.h"
#include "GeneralCommunicationCallback.h"


GeneralCommunicationCallback::GeneralCommunicationCallback(void (*pCallbackFunc)(const char message[])) {
  _pCallbackFunc = pCallbackFunc;
}

/**
 * @brief Reads characterstic and calls callback function.
 */
void GeneralCommunicationCallback::onWrite(BLECharacteristic *pCharacteristic){
  std::string const value = pCharacteristic->getValue();

  char* message = strdup(value.c_str());
  if (value.length() > 0) {
    _pCallbackFunc(message);
  }
  free(message);
}
