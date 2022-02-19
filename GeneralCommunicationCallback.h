/*
  GeneralCommunicationCallback.h - Library for configuring the wifi credentials 
  of a esp32 controller via BLE.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: GNU General Public License v3 (GPL-3).
*/

#include "BLECharacteristic.h"

#ifndef GeneralCommunicationCallback_h
#define GeneralCommunicationCallback_h

class GeneralCommunicationCallback: public BLECharacteristicCallbacks {
  public:
	  GeneralCommunicationCallback();
	
  private:
    void onWrite(BLECharacteristic *pCharacteristic);
};

#endif