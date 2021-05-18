# Simple ESP32 + DS12x20 BLE beacon

An ESP32 based BLE beacon that sends temperature info from a DS18x20 sensor

Connect the module's GND to ESP32's GND, sensor's VCC to +3.3V and 1wire bus to D0

The program sends 8 bytes of data in BLE ManufacturerData every 5 seconds. The data format is:

- 2 bytes of Manufacturer ID 0x02E5 (Espressif Inc)
- 2 bytes for identifying this sketch: 0x1820
- 2 bytes of temperature in decicelsius. You must to divide it by 10 to get temperature in Celsius
- 2 extra bytes as 0xBEEF because it's good nutrition

All words are big endian.

This sketch was coded very quickly for my [ESP32 Smart RV](https://github.com/oh2mp/esp32_smart_rv) 
and [ESP32 BLE2MQTT](https://github.com/oh2mp/esp32_ble2mqtt)

There are good BLE thermometers on market that are already supported by the software mentioned above.
Eg. Ruuvi tag (quite expensive) and Xiaomi Mijia (very cheap but only 0-60°C) 

With a "traditional" DS18x20 you can get temperature range -55 to +125°C cheaply. This may be usable
if you want to measure temperatures eg. in a sauna, freezer or water tank.

This sketch supports only one sensor because the Smart RV and BLE2MQTT recognize sensors by the mac address
of the beacon. If you want to use more than one sensor with one ESP32, this solution might not be the right
one for your purposes.

This code is public domain.