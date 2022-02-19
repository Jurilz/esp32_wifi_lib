/*
  WifiConfiguratorCallback.h - BLECharacteristicCallbacks for ESP32WifiConfigurator library.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: GNU General Public License v3 (GPL-3).
*/

#include "BLECharacteristic.h"

#ifndef WifiConfiguratorCallback_h
#define WifiConfiguratorCallback_h

class WifiConfiguratorCallback: public BLECharacteristicCallbacks {
  public:
	  WifiConfiguratorCallback(void (*pCallbackFunc)(const char ssid[], const char pw[]));
	
  private:
    void onWrite(BLECharacteristic *pCharacteristic);
    void (*_pCallbackFunc)(const char ssid[], const char pw[]);
};

#endif