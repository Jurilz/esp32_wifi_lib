/*
  ESP32WifiConfigurator.h - Library for configuring the wifi credentials 
  of a esp32 controller via BLE.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: GNU General Public License v3 (GPL-3).
*/

#include "BLECharacteristic.h"

#ifndef WifiConfigurationCallback_h
#define WifiConfigurationCallback_h

class WifiConfigurationCallback: public BLECharacteristicCallbacks {
  public:
	  WifiConfigurationCallback();
	
  private:
    void onWrite(BLECharacteristic *pCharacteristic);
};

#endif