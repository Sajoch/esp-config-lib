## Introduction
Store data in EEPROM using Arudino framework. Use
[AT24C02](https://ww1.microchip.com/downloads/en/DeviceDoc/doc0180.pdf) with ESP8266 and configurate data you want to read.

## Configuration
File **data_format.json** have configuration of all data that can be accessed from this library. 

* A field defined as **AppName** with *type* **string** can be accessed in code by `config.GetAppName()` and it returns `std::string` - in EEPROM it is stored as length and byte array (it is not null terminated)
* A field defined as **ServerPort** with *type* **uint16** can be accessed in code by `config.GetServerPort()` and it returns `uint16_t`
* A field defined as **AuthKey** with *type* **secure-string** can be accessed in code by `config.GetAuthKey()` with *type* `std::string` - in EEPROM it is stored as byte array with every byte xored.

## Usage
* Use docker to build image of it or run 
```bash
node generate.js
```
* Copy content of *build* directory to *libs/esp-config-lib* in a PlatformIO project
* Use example code to interact with this library
```c++  
#include <cinttypes>
#include <Config.h>

const uint8_t eepromAddress = 0x50; // TODO
const uint8_t eepromVersion = 1;
const uint8_t xorByte = 0x69;
TwoWire peripherialBus;

void setup() {  
	peripherialBus.begin(D1, D2);
	Config config(&peripherialBus, eepromAddress, 
				  eepromVersion, xorByte);
				  
	if (!config.IsValid()) {
		Serial.println("EEPROM error");
		while (1)
			yield();
	}
	//Read data from EEPROM
	setup_wifi(config.GetWiFiSSID(), config.GetWiFiPassword());
}

```
