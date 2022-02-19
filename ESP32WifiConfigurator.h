/*
  ESP32WifiConfigurator.h - Library for configuring the wifi credentials 
  of a esp32 controller via BLE.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: GNU General Public License v3 (GPL-3).
*/

#include "Arduino.h"
#include "BLEDevice.h"
#include "GeneralCommunicationCallback.h"
#include "WifiConfiguratorCallback.h"

#ifndef ESP32WifiConfigurator_h
#define ESP32WifiConfigurator_h

class ESP32WifiConfigurator {

    private:
        char* _deviceName;

        static boolean _bleServerStarted;
        
        BLECharacteristic* _wifiConfiguration;
        
        void setUpBLECharacteristics(BLEService* wifiConfigureService);

        void startBLE();

        String scanForWiFis();

    public:
        ESP32WifiConfigurator();
        
        static BLECharacteristic* _availableWifiNetworks;

        void startWifiConfigurator();

        static void stopBLE();

        static boolean connectToWiFi(const char ssid[], const char pw[]);
};
#endif