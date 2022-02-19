/*
  GeneralCommunicationCallback.h - BLECharacteristicCallbacks for ESP32WifiConfigurator library.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: GNU General Public License v3 (GPL-3).
*/

#include "BLECharacteristic.h"

#ifndef GeneralCommunicationCallback_h
#define GeneralCommunicationCallback_h

class GeneralCommunicationCallback: public BLECharacteristicCallbacks {
  public:
	  GeneralCommunicationCallback(void (*pCallbackFunc)(const char message[]));
	
  private:
    void onWrite(BLECharacteristic *pCharacteristic);
    void (*_pCallbackFunc)(const char message[]);
};

#endif