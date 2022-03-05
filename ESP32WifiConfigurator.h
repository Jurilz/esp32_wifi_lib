/*
  ESP32WifiConfigurator.h - Library for configuring the wifi credentials 
  of a esp32 controller via BLE.
  Author: J. Lozowoj
  Created on: 18.02.2021.
  Licence: Apache License 2.0 (Apache-2.0).
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

        static BLECharacteristic* _availableWifiNetworks;     

        static void wifiSuccessCallback(const char ssid[], const char pw[]);

        static void connectionClosedCallback(const char message[]);

        static boolean connectToWiFi(const char ssid[], const char pw[]);
        
        void setUpBLECharacteristics(BLEService* wifiConfigureService);

        void startBLE();

        String scanForWiFis();

        static void stopBLE(void* parameter);

    public:
      ESP32WifiConfigurator(char deviceName[]);

      void startWifiConfigurator();

};
#endif
