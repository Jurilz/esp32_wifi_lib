# ESP32 WiFi Configuration Lib

This Library enables you to set WiFi credentials for an ESP32 Controller via a BLE connection. After an WiFi connection is established the BLE server is shut down.

It also scans the available WiFi networks and publishes them via the Available Network Characteristics.

## Usage

```c++
#include "ESP32WifiConfigurator.h"

ESP32WifiConfigurator wifiConfigurator("myBLEDevice");

void setup()
{
  wifiConfigurator.startWifiConfigurator();
}
```

## API

On `startWifiConfigurator()` a BLE Server with the name given in the constructor ist started.

It advertises a BLE Service with the UUID `4fafc201-1fb5-459e-8fcc-c5c9c331914b`.

The BLE Service uses two BLE Characterstics for communication with a BLE Client.

### Available Networks Characteristics
The *Available Networks Characteristics* can be found via the UUID `beb5483e-36e1-4688-b7f5-ea07361b26a8`. 

First a list of all avaible WiFi networks are published via them. The SSIDs are seperated by a newline character (`\n`).

They also listen to a *connection closed* (CLOSED) notification and shutdown the BLE server on receipt.

### WiFi Setup Characteristics

The *WiFi Setup Charactersistics* can be found via the UUID `59a3861e-8d11-4f40-9597-912f562e4759`.

They expect the WiFi name (SSID) and the password seperated by a newline character (`\n`) and encoded as a byte array. An attempt is then made to establish a connection with the given WiFi.

On succes a notification (SUCCESS) is written to the *Available Networks Characteristics*.


## General Information

For development an ESP-Wroom-32 Controller was used.

The memory consumption is about 1.3 MB mostly due to usage of `WiFI.h` (0.5 MB) and `BLEDevice.h` (0.8 MB) libraries.

The applied partion scheme was: `Huge App (3MB NO OTA/1MB SPIFFS)`.

## Used Dependencies

* Arduino.h
* BLEDevice.h
* BLEUtils.h
* BLEServer.h
* BLE2902.h
* WiFi.h

## Documentation

More information can be found [here](https://jurilz.github.io/wifi-esp32-docs/).


## Licence
Apache License 2.0 (Apache-2.0)