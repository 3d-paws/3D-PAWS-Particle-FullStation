# Sensor Information
[←Top](../README.md)<BR>

Sensors
| Sensor Model                  | Type                    | Description                                                                                              | Company URL                             |
|-------------------------------|-------------------------|----------------------------------------------------------------------------------------------------------|------------------------------------------|
| Adafruit HTU21DF              | Temp/Humidity           | Digital I2C sensor, ±2% RH, ±0.3°C, compact, low power.                                                  | [Adafruit HTU21DF](https://www.adafruit.com/product/1899)   |
| Adafruit MCP9808              | Temperature             | High-precision digital temp sensor, ±0.25°C, 2.7-5.5V, I2C.                                              | [Adafruit MCP9808](https://www.adafruit.com/product/1782)   |
| Adafruit BME280               | Temp/Humidity/Pressure  | Measures temp, humidity, pressure, I2C/SPI, -40 to +85°C, 0-100% RH, 1.8-5V.                             | [Adafruit BME280](https://www.adafruit.com/product/2652)    |
| Adafruit BMP280               | Temp/Pressure           | Measures pressure (300–1100 hPa), temperature, I2C/SPI, small, low-power.                                | [Adafruit BMP280](https://www.adafruit.com/product/2651)    |
| Adafruit BMP3XX               | Temp/Pressure           | Gen3 barometric, high-precision, ±0.5m alt., I2C/SPI.                                                    | [BMP388/BMP3XX](https://shop.pimoroni.com/en-us/products/adafruit-bmp388-precision-barometric-pressure-and-altimeter) |
| Adafruit SHT31                | Temp/Humidity           | High accuracy, ±2% RH, ±0.3°C, I2C, fast response, 2.4–5.5V.                                             | [Adafruit SHT31](https://www.adafruit.com/product/2857)     |
| Adafruit VEML7700             | Ambient Light           | Precise lux (0–120klux), I2C, auto-range, digital output, 3.3/5V.                                        | [Adafruit VEML7700](https://www.adafruit.com/product/4162)  |
| Adafruit SI1145               | Light/UV/Proximity      | I2C, measures visible, IR, and digital UV index, 3-5V.                                                   | [Adafruit SI1145](https://www.adafruit.com/product/1777)    |
| Adafruit PM25AQI              | PM2.5 Air Quality       | Measures particulate (0.3–10um), I2C (fixed 0x12), 0-500μg/m³ range, 5V.                                 | [PM25AQI](https://www.adafruit.com/product/4505)            |
| Adafruit HDC302x              | Temp/Humidity           | High-accuracy I2C digital sensor (variant of HDC3021, HDC3022).                                          | [Adafruit HDC302x](https://www.adafruit.com)                |
| Adafruit LPS35HW              | Pressure                | Robust water-resistant MEMS pressure sensor, I2C/SPI, oil/water resistant.                               | [LPS35HW](https://www.adafruit.com)                        |
| DFRobot_B_LUX_V30B            | Ambient Light           | Measures up to 200klx, wide range, digital I2C, 3.3-5V.                                                  | [DFRobot Wiki](https://wiki.dfrobot.com/Ambient_Light_Sensor_0_200klx_SKU_SEN0390) |
| Honeywell HIH8000             | Temp/Humidity           | Reliable, digital I2C, ±3% RH, ±0.5°C, industrial grade.                                                | [Honeywell HIH8000](https://sensing.honeywell.com)          |
| Tinovi LeafSens               | Leaf Wetness/Temp       | Non-contact capacitive leaf wetness + temperature, digital.                                              | [Tinovi LeafSens](https://www.tinovi.com/leafsens)          |
| Tinovi i2cArduino             | Soil Moisture/Temp      | Capacitive soil moisture + temperature, I2C.                                                             | [Tinovi i2cArduino](https://www.tinovi.com/i2carduino)      |
| Tinovi i2cMultiSm             | Multi Soil/Temp         | 5-point capacitive moisture + temperature, I2C, multi-depth.                                             | [Tinovi i2cMultiSm](https://www.tinovi.com/i2cmultism)      |
| AS5600                        | Magnetic Encoder        | Rotational angle/magnetic position sensor, I2C.                                                          | [AS5600 Sensor](https://www.ams.com/as5600)                 |
| AS5600L                       | Magnetic Encoder        | Same as AS5600, but designed for integration in new designs (ex: Particle Muon).                         | [AS5600L Sensor](https://www.ams.com/as5600l)               |
| SS451A (Rain Gauge)           | Hall Effect Switch      | Digital magnetic field sensor, rugged, used as rain gauge switch.                                        | [SS451A Datasheet](https://sensing.honeywell.com)           |
| SS451A (Anemometer)           | Hall Effect Switch      | Used as wind anemometer sensor (rotation detection).                                                     | [SS451A Datasheet](https://sensing.honeywell.com)           |
| TMP112A                       | Temperature             | Precision digital temp sensor, on-board Particle Muon.                                                   | [TMP112A Sensor](https://www.ti.com/product/TMP112A)        |

---
### Wind
#### Collecting Wind Data
- **Wind_SampleSpeed()** – Returns the wind speed based on interrupt counts and the duration between calls.  
- **Wind_SampleDirection()** – Reads wind direction via I²C from the AS5600(L) sensor.  
- **Wind_TakeReading()** – Called every second. It collects wind direction and wind speed, then stores the samples in a circular buffer of 60 buckets.  

#### Creating the 1 Minute Wind Observations
- **Wind_DirectionVector()** – Uses the 60-sample buffer of wind directions and wind speeds (where wind speed > 0) to compute and return a wind vector.  
- **Wind_SpeedAverage()** – Returns the average wind speed from the 60-sample buffer.  
- **Wind_GustUpdate()** – Finds the highest three consecutive wind speed samples in the buffer, averages them to calculate the wind gust, and updates `wind.gust` and `wind.gust_direction`. `Wind_GustUpdate()` must be called before `Wind_Gust()` or `Wind_GustDirection()`.  
- **Wind_Gust()** – Returns `wind.gust`.  
- **Wind_GustDirection()** – Returns `wind.gust_direction`.  

---
### PM25AQI - I2C - Air Quality Sensor
#### Concentration Units (standard)
- pm10_standard: Standard PM1.0 (µg/m³)  
- pm25_standard: Standard PM2.5 (µg/m³)  
- pm100_standard: Standard PM10.0 (µg/m³)
#### Concentration Units (environmental)
- pm10_env: Environmental PM1.0 (µg/m³)  
- pm25_env: Environmental PM2.5 (µg/m³)  
- pm100_env: Environmental PM10.0 (µg/m³)  
#### Particle Variables - Not Reporting
- 03um: ≥ 0.3 µm per 0.1 L of air  
- 05um: ≥ 0.5 µm per 0.1 L of air  
- 10um: ≥ 1.0 µm per 0.1 L of air  
- 25um: ≥ 2.5 µm per 0.1 L of air  
- 50um: ≥ 5.0 µm per 0.1 L of air  
- 100um: ≥ 10.0 µm per 0.1 L of air  

#### Notes
- pms = Particulate Matter Standard  
- pme = Particulate Matter Environmental  

#### Variable Tags
- pm1s10, pm1s25, pm1s100  
- pm1e10, pm1e25, pm1e100  

---
### MaxBotix Distance Sensors
#### 5-Meter Sensors  
(MB7360, MB7369, MB7380, MB7389)  
Scale factor: (Vcc / 5120) per 1 mm  

- Particle (12-bit, 0–4095): Range 0–5119 mm, resolution 1.25 mm per unit  
- Feather (10-bit, 0–1023): Range 0–5119 mm, resolution 5 mm per unit  

#### 10-Meter Sensors  
(MB7363, MB7366, MB7383, MB7386)  
Scale factor: (Vcc / 10240) per 1 mm  

- Particle (12-bit, 0–4095): Range 0–10239 mm, resolution 2.5 mm per unit  
- Feather (10-bit, 0–1023): Range 0–10239 mm, resolution 10 mm per unit  

### I2C Mux and Multiple Tinovi Soil Moisture/Temp Sensors

MUX:INIT
MUX OK
MUX:SCAN
  CH-0 TSM NF
  CH-1 TSM NF
  CH-2 TSM NF
  CH-3 TSM NF
  CH-4 TSM NF
  CH-5 TSM NF
  CH-6 TSM NF
  CH-7.0 TSM OK
Where the .0 is the first occurance of a discovered sensor on this channel.  In the future we ma be supporting other sensors on the mux

 *  MUX sensors where [1-8] is the MUX channel
 *  tsmt-[1-8]    Tinovi Soil Moisture temperature
 *  tsme25-[1-8]  Tinovi Soil Moisture e25
 *  tsmec-[1-8]   Tinovi Soil Moisture ec
 *  tsmvwc-[1-8]  Tinovi Soil Moisture vwc



