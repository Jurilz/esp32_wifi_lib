#include <BLEDevice.h>

class ESP32WifiConfigurator {

    private:
        char[] _SERVICE_UUID;
        char[] _AVAILABE_WIFI_NETWORKS_CHARACTERISTIC_UUID;
        char[] _WIFI_SETUP_CHARACTERISTIC_UUID;
        int _WAIT_TIME;   

        BLECharacteristic* _availableWifiNetworks;
        BLECharacteristic* _wifiConfiguration;
        boolean _bleServerStarted;

        void connectToWiFi(const char ssid[], const char pw[]);

        class WifiConfigurationCallback: public BLECharacteristicCallbacks;

        void setUpBLECharacteristics(BLEService* wifiConfigureService);

        void startBLE();

        void updateAvailableWifiNetworks();

        void stopBLE();

        String scanForWiFis();

    //TODO: maybe class constructor
    public:
        void startWifiConfigurator(int TIME_INTERVAL);
};