/*

  ESP32WifiConfigurator.cpp - Library for configuring the wifi credentials 
  of a esp32 controller via BLE.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: GNU General Public License v3 (GPL-3).
*/


#include "BLECharacteristic.h"
#include "ESP32WifiConfigurator.h"
#include "GeneralCommunicationCallback.h"

#define _CLOSED   "CLOSED"


// Inversion of Controll, give closed message
// and callback

GeneralCommunicationCallback::GeneralCommunicationCallback() {}

void GeneralCommunicationCallback::onWrite(BLECharacteristic *pCharacteristic){
  std::string const value = pCharacteristic->getValue();

  char* message = strdup(value.c_str());
  if (value.length() > 0) {
    for (int i = 0; i < value.length(); i++) {
      Serial.print(value[i]);
    }
    Serial.println();
    if (String(message).equals(_CLOSED)) {
      ESP32WifiConfigurator::stopBLE();
    }
  }
  free(message);
}