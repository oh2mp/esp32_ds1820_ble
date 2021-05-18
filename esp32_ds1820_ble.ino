/*
 * ESP32 + DS18x20 BLE beacon
 *
 * See https://github.com/oh2mp/esp32_ds1820_ble
 *
 */
 
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEBeacon.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 0            // Data pin D0 for onewire

BLEAdvertising *advertising;
int16_t temperature;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress deviceAddress;

/* ----------------------------------------------------------------------------------
 * Set up data for advertising
 */ 
void set_beacon() {
    BLEBeacon beacon = BLEBeacon();
    BLEAdvertisementData advdata = BLEAdvertisementData();
    BLEAdvertisementData scanresponse = BLEAdvertisementData();
    
    advdata.setFlags(0x06); // BR_EDR_NOT_SUPPORTED 0x04 & LE General discoverable 0x02

    std::string mfdata = "";
    mfdata += (char)0xE5; mfdata += (char)0x02;  // Espressif Incorporated Vendor ID = 0x02E5
    mfdata += (char)0x20; mfdata += (char)0x18;  // Identifier for this sketch is 0x1820
    mfdata += (char)(temperature & 0xFF);        // LSB for temperature
    mfdata += (char)(temperature >> 8);          // MSB for temperature
    mfdata += (char)0xBE; mfdata += (char)0xEF;  // Beef is always good nutriment
  
    advdata.setManufacturerData(mfdata);
    advertising->setAdvertisementData(advdata);
    advertising->setScanResponseData(scanresponse);
}

/* ---------------------------------------------------------------------------------- */
void setup() {
    sensors.begin();
    delay(100);
    sensors.setResolution(12);
    sensors.getAddress(deviceAddress,0);
                
    for (uint8_t j = 0; j < 8; j++) {
                Serial.printf("%02X", deviceAddress[j]);
    }
    
    BLEDevice::init("ESP32+DS1820");
    advertising = BLEDevice::getAdvertising();
}
/* ---------------------------------------------------------------------------------- */

void loop() {
    sensors.requestTemperatures();
    temperature = sensors.getTemp(deviceAddress);

    // If no error in data, then convert to decicelsius and advertise
    if (temperature != 0 && temperature != 0xaa && temperature != 0x2a80) {
        if (temperature > 0) {
            temperature = temperature/12.8+.5;
        } else {
            temperature = temperature/12.8-.5;
        }
        set_beacon();
        advertising->start();
        delay(100);
        advertising->stop();
    }
    
    delay(4500);
    
    // Reboot once in hour to be sure    
    if (millis() > 3.6E+6) {
        ESP.restart();
    }
}
/* ---------------------------------------------------------------------------------- */
