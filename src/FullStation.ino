PRODUCT_VERSION (43);
#define COPYRIGHT "Copyright [2025] [University Corporation for Atmospheric Research]"
#define VERSION_INFO "FS-251114v43"

/*
 *======================================================================================================================
 * FullStation (FS)
 *   Board Type : Particle Boron https://docs.particle.io/reference/datasheets/b-series/b404x-datasheet
 *   Board Type : Particle Argon https://docs.particle.io/reference/datasheets/wi-fi/argon-datasheet
 *   Board Type : Particle Muon  https://docs.particle.io/reference/datasheets/m-series/muon-datasheet
 * 
 *   Description: Monitor 3D-PAWS Full Station and transmit data to Particle Cloud
 *   Author: Robert J Bubon
 *   Date:   2021-01-14 RJB Initial Development
 *           2021-02-10 RJB Work on SD code area
 *           2021-03-08 RJB LoRa added, 15min samples, Cell Power Off, Wind Observations Corrected
 *           2021-03-13 RJB Reworked N2S sending
 *           2021-04-20 RJB Clear Rain Gauge Counter
 *           2021-06-04 RJB Added Particle Theads
 *           2021-06-10 RJB Added Countdown timer on jumper, Modified Network Disconnect and Power Down disconnect
 *           2021-06-14 RJB When jumper set unit will continue the network connection to Particle, breathe cyan
 *           2021-06-21 RJB Correct bug in network connect and handle BME returning NAN.
 *           2021-07-12 RJB Always Connected and Debug Added
 *           2021-07-21 RJB Updated Libraries after HTU has infinate loop in code
 *           2021-10-06 RJB Version 2
 *                          Merged 15MC changes in, removed unneeded debug, 
 *                          formalized the Always Connected branch removed the 15m disconnect code 
 *           2021-10-22 RJB Version 3 
 *                          Remote Reboot Must pass "NOW" from Particle IDE
 *                          Battery Charger Fault Register, Variable name "CFR"
 *                          Adafruit BMP388 Support Added
 *           2022-01-03 RJB Version 4
 *                          Added Support for 2 MCP sensors
 *           2022-01-29 RJB Version 5
 *                          Spelling Corrections
 *                          Added support for Grove Shield
 *                          PIN CHANGES FOR GROVE SHIELD
 *                             LORA - D7 moved to A1
 *                             Rain - D8 moved to A3
 *                             Anemometer - D6 moved to A2
 *          2022-04-10  RJB Version 6
 *                          Added support for 3rd Party Sim
 *                          Changed guage to gauge - proper spelling
 *                          Added i2c power reset relay board ctrl on A0 
 *          2022-08-01  RJB Version 7
 *                          Wind direction check for 0-360 value
 *                          Nan Check on t,h,p sensor values, return -999.99 for bad t and 0.0 for h,p
 *          2022-09-23 RJB Version 8
 *                          Modified SystemStatusBits handling of SSB_FROM_N2S and SSB_N2S
 *                          Added Support for 8 line OLED - Special Compile
 *          2022-10-17 RJB Version 9
 *                          Increased N2S file size to 2 days and set float precision to 1/10th.
 *          2022-10-24 RJB  Support of Argon board added
 *                            Note: PMIC (BQ24195) not available on Argon (Gen 3)
 *                                  Argon uses XC6802
 *                                  No power off on low battery support with Argon
 *                          Wifi AuthType, SSID, Password stored in file WIFI.txt
 *          2022-10-31 RJB Larger OLED for Argon
 *          2022-11-03 RJB WIFI.TXT changed from space to comma as the separator
 *                         Wait for serial While loop put back
 *                         Added a 120s timeout to wait for serial.
 *          2022-11-17 RJB Merged changes from MKR NB1500 code base. A lot of the initialization
 *                         was moved out of setup() to function calls for the initialization.
 *                         Argon only - Added a System.reset() after 1 hour with out a WiFi Connection
 *          2022-12-21 RJB added clearing of SSB_HTU21DF in JPO_ClearBits()
 *          2023-01-13 RJB Added MMA Barbados Product ID
 *          2023-03-14 RJB Added VEML7700 light sensor support
 *          2023-04-19 RJB Added Support for Watchdog Monitor Relay Board.
 *                         Moved from 3.0.0 Firmware to 4.0.2 
 *          2023-04-19 RJB Version 11 
 *                         Clear SI and Lux Status bits if not found
 *          2023-06-07 RJB Rain gauge inturrupts turn on LED, Background work shuts it off
 *          2023-06-12 RJB Compiled for firmware 4.1.0
 *          2023-07-10 RJB Added Min Max Quality Controls to sensor values
 *          2023-07-11 RJB Added Daily Reboot
 *          2023-07-25 RJB Added Particle WITH_ACK check and refresh of wind data after sending
 *          2023-07-30 RJB Fixed bug to get RTC updated from network time
 *          2023-08-09 RJB Refresh of wind data only happend if it took 2 seconds or more to transmit.
 *                         Added EEPROM of N2S file position pointer and Total Rain Today and Prior Day
 *          2023-08-24 RJB Removed code supporting the non Grove shield option
 *          2023-08-31 RJB Moved to Particle function RESET (NOW) to DoAction(REBOOT). Added support
 *                         for DoAction(CRT) to Clear Rain Totals from EEPROM
 *          2023-10-05 RJB Version 18
 *                         Moved Serial Console Pin A4 to D8
 *                         Added Dynamic configuration for pin A4
 *                            If SD file A4DIST.TXT exists then Distance Gauge
 *                            If SD file A4RAIN.TXT exists then 2nd Rain Gauge
 *                         Added support for setting pin A4 from Particle
 *                           DoAction Commands  A4DIST, A4RAIN, A4CLR - Requires DoAction(REBOOT) after!
 *          2023-10-19 RJB Version 19
 *                         Added support for determining if oled is 32 or 64 pixels high
 *                         Added support for Air Quality Sensor PM25AQI
 *          2023-10-25 RJB Version 21 Actual Release
 *                         Bug Fix on Distance reading. 
 *                         Added Dynamic configuration for Distance Sensor and related distance calc
 *                               If SD file 5MDIST.TXT exists then distance reading is divided by 8
 *                               If SD file 5MDIST.TXT does NOT exist then distance is divided by 4, the default
 *                         Added support for setting distance meter type from Particle
 *                           DoAction Command 5MDIST - No reboot needed if return code to Particle is 0
 *                           If you send/resend A4DIST, the 5MDIST.TXT is deleted. 
 *          2023-12-19 RJB Version 22
 *                         Modified WiFi Credentials Setup 
 *                         Now save unreported rain before daily reboot
 *          2024-01-05 RJB Code fix for Daily Totals rotation
 *          2024-01-14 RJB Version 23
 *                         Code Fix added eeprom_valid = true; in the Initialize
 *          2024-01-31 RJB Version 24
 *                         Special for stations using A3 to rain/D8 and A2 to Wind/D6
 *          2024-02-23 RJB Version 25
 *                         Corrected Distance calculation
 *                         If file exists then we multiply by 1.25. If no file, then we multiply by 2.5 for the 10m Sensor
 *          2024-05-29 RJB Version 26
 *                         Bug fixes in WiFiChangeCheck() and SimChangeCheck()
 *                         Needed to add #if PLATFORM_ID == PLATFORM_BORON around the getting of IMSI information
 *                         Added output of the length of the particle payload when we send
 *                         Added support for wet bulb temperature (wbt)
 *          2024-06-11 RJB Version 27/28
 *                         Split code up into include files
 *                         Added CONFIG.TXT support
 *                         Moved LoRa configs to CONFIG.TXT
 *                         Added support for heat index (hi)
 *                         Added support for WBGT
 *                         Modified SF.h - isValidHexString(), hexStringToUint32(), SDC.h - SD_findKey()
 *          2024-06-11 RJB Version 29 Released
 *                         Bug Bix had = and not == on QC check for HI, WBT, WBGT
 *                         Bug Fix had 0.034 and and 0.0034 in WBGT eq.
 *                         Removed QC check for WBGT
 *                         Change QC for heat index to display a temperature
 * 
 *          Version 30 Released on 2024-09-14
 *          2024-06-20 RJB Now prints message when CONFIG.TXT not found on SD
 *                         Added Apache Copyright License
 *                         Added printing of Copyright to Serial Console
 *          2024-07-23 RJB Added dg_adjustment = 2.5 when removing 5MDIST.TXT
 *          2024-08-22 RJB Addded support for Globe temperature.
 *                         Added MCP_3 dedicated to Globe temperature will report as gt1
 *                         gt1 will be used when calling wbt_calculate()
 *          2024-09-09 RJB Added MCP_4 dedicated to Globe temperature will report as gt2
 *          2024-09-11 RJB Station Monitor changed to provide wind speed not interrupt count
 *                         When setting SIM to INTERNAL we now set changed = true to
 *                         report success and reboot message.
 *                         Changed Td to Ta in the wbgt_using_wbt() function
 *          2024-09-14 RJB Modified WiFi Support for UNSEC allowing no password Ex:  "UNSEC,ssid,"
 * 
 *          Version 31 Released on 2024-10-04
 *          2024-10-02 RJB Tag name changes rg2t -> rgt2 and rg2p -> rgp2
 * 
 *          Version 32 Released on 2024-10-07
 *          2024-10-07 RJB Improved hi_calculate() function. 
 * 
 *          Version 33 Released on 2024-11-04 (Early Relese to Argentina)
 *          2024-11-04 RJB Added INFO_Do() at boot and when called via Do_Action with "INFO"
 *                         Added Do_Action feature "SEND" to send OBS that are cued
 *                         Added support for HDC302x sensor, Reports as hdt1,hsh1,hdt2,hdh2
 *                         Added support for 5,10,15(default) minute obs transmit intervals.
 *                         Moved reporting of SystemStatusBits to after we have read all the sensors in OBS_Do()
 * 
 *          Version 34 Released on 2024-11-05 (Early Relese to Argentina)
 *          2024-11-05 RJB Discovered BMP390 first pressure reading is bad. Added read pressure to bmx_initialize()
 *                         SEE: https://forums.adafruit.com/viewtopic.php?t=209906
 *                         Bug fixes for 2nd BMP sensor in bmx_initialize() using first sensor data structure
 *                         Now will only send humidity if bmx sensor supports it.
 * 
 *          Version 35 Released on 2024-11-13
 *          2024-11-09 RJB HDC302x added sensor read in the sensor init
 *          2024-11-13 RJB DFRobot_B_LUX_V30B support added  0 lumens to 200,000 lumens. Reports a blx
 *                         Changed vmel lx tag vlx when sending to Particle
 *   
 *          Version 37 Released on 2024-12-11
 *          2024-11-19 RJB Store INFO information in INFO.TXT. Every INFO call will overwrite the files content.
 *          2024-11-26 RJB Some minor changes made in the code
 *                         INFO will now report DailyRebootCountDownTimer as drct
 *                         Improved blx_takereading() by adding a 1s timer to wait for the 4 bytes. Avoids infinite loop.
 *                         Modified Adafruit_HDC302x/src/Adafruit_HDC302x.cpp library. So sendCommandReadTRH() avoids infinite loop.
 *          2024-12-04 RJB Updated Adafruit_VEML7700 library to 2.1.6
 *          2024-12-06 RJB Added support for lps35hw pressure and temperature lpp1,lpt1,lpp2,lpt2
 *                         Upgrading to use deviceOS 6.1.1
 *          2024-12-09 RJB INFO msg now sent before powering down do to low lipo battery.
 * 
 *          Version 38 Released on 2025-03-17 (Version 39 FEWSNET)
 *          2025-01-07 RJB Moved LORA_IRQ_PIN from A5 to D6.
 *          2025-01-14 RJB Rebuilt the LoRa message handling. 
 *                         We now receive LoRa messages in JSON format
 *                         We now store then forward to Particle LoRa message types: INFO, LR.
 *                         On LoRa messages received, the JSON portion is forwarded 
 *                         Added RG and associated pin to INFO messages  
 *                         We now call OBS_PublishAll() before daily rebooting and powering down on low battery
 *                         We moved low battery powerdown from 10% to 15%.
 *          2025-01-21 RJB Added support for A4 to be configured for raw readings (Simple average of 5 samples) 10ms apart
 *                           DoAction A4RAW. Reports to Particle as a4r 
 *                         Added support for A5 to be configured for raw readings (Simple average of 5 samples) 10ms apart
 *                           DoAction A5RAW and A5CLR. Reports to Particle as a5r 
 *          2025-01-23 RJB Added support for Tinovi moisture sensors (Leaf, Soil, Multi Level Soil) 
 *          2025-03-17 RJB Switched Heat Index Temp, Wet Bulb Temp, Wet Bulb calcs to use sht1 temp from mcp1 temp
 *     
 *          Version 40 Released on 2025-09-11
 *          2025-04-08 RJB Removed the check for serial console in SimChangeCheck()
 *          2025-04-13 RJB Reworked the handling of pin names. New function in PS.h called GetPinName()
 *                         INFO will report "lora:NF" when LoRa is not found
 *                         INFO Serial Console Enable now reports as "scepin(D8)":"DISABLED"                         
 *          2025-07-17 RJB Support for Muon - PLATFORM_MSOM
 *                         Updated SdFat Library from 1.0.16 to 2.3.0
 *                         Added support for TMP117 to support Muon built in Temperature sensor
 *                           Reports as PMTS = Particle Muon Temperature Sensor 
 *                         Bug, Left the 0 && in prior release - if (0 && countdown && digitalRead(SCE_PIN) == LOW)
 *                         Added resetReason to be printed at startup and in INFO
 *                         Added GNSS to INFO
 *                         Reworked when we initialize wind and make first obs at startup
 *                         AS5600 is same i2c addrss as as fuel gauge 0x36. Muon will need to use AS5600L for wind direction.
 *                         Added Support for AS5600L just for the Muon. Can not have a HTU21DF connected, same i2c address as fuel gauge
 *                         Added Support for Observation Interval and Transmit time controls
 *                                TXI5M  Set 1 Minute Observations, Transmit Interval to 5 Minutes (TXI5M.TXT)- Already in place
 *                                TXI10M Set 1 Minute Observations, Transmit Interval to 10 Minutes (TXI10M.TXT)- Already in place
 *                                TXI15M Set 1 Minute Observations, Transmit Interval to 15 Minutes (NO FILE) - Already in place Default
 *                                OBI5M  Set 5 Minute Observations, Transmit Interval to 5 Minutes (OBI5M.TXT)
 *                                OBI10M Set 10 Minute Observations, Transmit Interval to 10 Minutes (OBI10M.TXT)
 *                                OBI15M Set 15 Minute Observations, Transmit Interval to 15 Minutes (OBI15M.TXT)
 *                         Rename A4 option to OP1 = OP1DIST, OP1RAIN, OP1RAW, OP1CLR, OP1D5M; OP1DIST.TXT, OP1RAIN.TXT, OP1RAW.TXT, OP1D5M.TXT
 *                                A5 option to OP2 = OP2RAW, OP2CLR; OP2RAW.TXT
 *                         Muon can not support Tinovi Leaf Wetness sensor i2c 0x61 conflicts with KG200Z LoRaWAN radio - removed from Muon compile
 *                         Muon WiFi Enable, create WIFI.TXT with 1 line inside with the following format MUON,ssid,password
 *          2025-08-27 RJB Missed reporting sensor tmsms5.
 *          2025-08-27 RJB Support added to change FS to a Air Quality Station - with power management of the AQS.
 *                         OPTAQS.TXT - Enables Air Quality Station. Pin A5 is wired to SET pin on sensor.
 *                                      SET pin, when low, puts sensor into sleep mode.
 *                                      Disable Wind, rain, and other OPT configs
 *                                      Files OP1DIST.TXT, OP1RAIN.TXT, OP1RAW.TXT, OP2RAW.TXT are ignored
 *          2025-08-29  RJB Added support to rename A4 and A5 files to OP1 and OP2 names
 *          2025-09-02  RJB Fixed issues with MUX and TSM.
 *          2025-09-05  RJB FS air quality now does a 1 minute average.
 *                          Added OPTAQS and OPTFS Particle Do Actions
 *          2025-09-06  RJB Bug fix Missed compile flags around the pin used for wind speed
 *          2025-09-07  RJB Improved EEPROM Valid check 
 *          2025-09-08  RJB Bug Fix in AQS taking a reading
 *                          Chhanged WBGT_exists is determined in Sensors.h and OBS.h
 *          2025-09-11  RJB In OBS fixed casting bug on rain collection. Added (float)
 *                          (rain > (((float)rgds / 60) * QC_MAX_RG))
 * 
 *          Version 42 Released on 2025-09-25
 *          2025-09-25  RJB Bug fix. Had obs tag names of length 6 bumped to 12.
 * 
 *          Version 43 Released on 2025-11-14
 *          2025-09-30  RJB Added support for ADS1115 (16bit ADC) i2c 0x48 and SP Lite2 Pyranometer from Kipp & Zonen
 *                          If we see the ADS1115 then we assume the Pyranometer is on adc0 
 *          2025-10-17  RJB AQS variable and function name cleanup
 *                          Bug fix on MUX TSM initialization of tsm_id.
 *          2025-10-27  RJB Code Cleanup, Added MSLP, Added Elev ELEV.TXT SETELEV:xxxx
 *          2025-11-10  RJB Added support for Evapotranspiration, Will report ET and ETc on the hour as Particle Event type ET
 *                          Code can be compiled out by disabling Macro ENABLE_Evapotranspiration
 *                          Corrected time reporting if AQS mode enabled but no sensor.
 *                          Added RTC to INFO
 *                          Removing sensors from system status bits. We have INFO.
 *                          Removed the I2C_Check_Sensors()
 *                          Distance Sensor raw reading "op1r" now has sensor type adjustment op1r = in_ReadAvg(OP1_PIN) * dg_adjustment;
 * 
 *  Muon Port Notes:
 *     PLATFORM_ID == PLATFORM_MSOM
 *     https://github.com/particle-iot/device-os/blob/develop/hal/shared/platforms.h
 * 
 *     Pin  Name
 *     LORA MODULE 
 *     40 D20 LORA_IRQ_PIN - G0 on LoRa board
 *     36 D3  LORA_SS
 *     38 D21 LORA_RESET
 *
 *     SERIAL CONSOLE 
 *     26  D17/A2 SCE_PIN
 * 
 *     SD CARD
 *     17  3v3
 *     19  SPI MOSI
 *     21  SPI MISO
 *     23  SPI SCK
 *     25  GND
 *     24  D29/A6  CS
 *     
 *     LED
 *     22  D22 LED_PIN
 * 
 *     GPS
 *     16 D24 Serial2 TX (UART transmit from MCU)
 *     18 D25 Serial2 RX (UART receive to MCU)
 * 
 *     GROVE PWM
 *     15  D27 RELAY/REBOOT PIN
 *     14  DO NOT USE
 * 
 *     GROVE PWM
 *     32 D5 Wind
 *     33 D4 Rain1
 * 
 *     GROVE D5
 *     29 A0/D19 OP1 PIN
 *     31 A1/D18 OP2 PIN
 *     
 *     Muon on board I2C addresses
 *     0x28: STUSB4500 USB-C power controller
 *     0x36: MAX17043 Fuel Gauge
 *     0x48: TMP112A temperature sensor
 *     0x61: KG200Z LoRaWAN radio
 *     0x69: AM1805 RTC/Watchdog
 *     0x6B: bq24195 Power Management IC (PMIC)
 *                 
 * NOTES:
 * When there is a successful transmission of an observation any need to send obersavations will be sent. 
 * On transmit a failure of these need to send observations, processing is stopped and the file is deleted.
 * 
 * Calibration
 * Adding serial console jumper after boot will cause sensors to be read every 1 second and values printed.
 * Removing serial console jumper will resume normal operation
 * 
 * Required Libraries:
 *  adafruit-HTU21DF        https://github.com/adafruit/Adafruit_HTU21DF_Library - 1.1.0 - I2C ADDRESS 0x40
 *  adafruit-BusIO          https://github.com/adafruit/Adafruit_BusIO - 1.8.2
 *  Adafruit_MCP9808        https://github.com/adafruit/Adafruit_MCP9808_Library - 2.0.0 - I2C ADDRESS 0x18
 *  Adafruit_BME280         https://github.com/adafruit/Adafruit_BME280_Library - 2.1.4 - I2C ADDRESS 0x77  (SD0 to GND = 0x76)
 *  Adafruit_BMP280         https://github.com/adafruit/Adafruit_BMP280_Library - 2.3.0 -I2C ADDRESS 0x77  (SD0 to GND = 0x76)
 *  Adafruit_BMP3XX         https://github.com/adafruit/Adafruit_BMP3XX - 2.1.0 I2C ADDRESS 0x77 and (SD0 to GND = 0x76)
 *  Adafruit_GFX            https://github.com/adafruit/Adafruit-GFX-Library - 1.10.10
 *  Adafruit_Sensor         https://github.com/adafruit/Adafruit_Sensor - 1.1.4
 *  Adafruit_SHT31          https://github.com/adafruit/Adafruit_SHT31 - 2.2.0 I2C ADDRESS 0x44 and 0x45 when ADR Pin High
 *  Adafruit_VEML7700       https://github.com/adafruit/Adafruit_VEML7700/ - 2.1.2 I2C ADDRESS 0x10
 *  Adafruit_SI1145         https://github.com/adafruit/Adafruit_SI1145_Library - 1.1.1 - I2C ADDRESS 0x60
 *  Adafruit_SSD1306        https://github.com/adafruit/Adafruit_SSD1306 - 2.4.6 - I2C ADDRESS 0x3C  
 *  Adafruit_PM25AQI        https://github.com/adafruit/Adafruit_PM25AQI - 1.0.6 I2C ADDRESS 0x12 - Modified to Compile, Adafruit_PM25AQI.cpp" line 104
 *  Adafruit_HDC302x        https://github.com/adafruit/Adafruit_HDC302x - 1.0.2 I2C ADDRESS 0x46 and 0x47 ( SHT uses 0x44 and x045)
 *  Adafruit_LPS35HW        https://github.com/adafruit/Adafruit_LPS35HW - 1.0.6 I2C ADDRESS 0x5D and 0x5C
 *  Adafruit_ADS1X15.h      https://www.adafruit.com/product/1085 - I2C ADDRESS 0x48 conflict with Muon TMP112A on board sensor
 *                          https://github.com/adafruit/Adafruit_ADS1X15
 * 
 *  DFRobot_B_LUX_V30B      https://github.com/DFRobot/DFRobot_B_LUX_V30B - 1.0.1 I2C ADDRESS 0x4A (Not Used Reference Only) SEN0390
 *                          https://wiki.dfrobot.com/Ambient_Light_Sensor_0_200klx_SKU_SEN0390
 * 
 *  RTCLibrary              https://github.com/adafruit/RTClib - 1.13.0
 *  SdFat                   https://github.com/greiman/SdFat.git - 1.0.16 by Bill Greiman 2.3.0
 *  RF9X-RK-SPI1            https://github.com/rickkas7/AdafruitDataLoggerRK - 0.2.0 - Modified RadioHead LoRa for SPI1
 *  AES-master              https://github.com/spaniakos/AES - 0.0.1 - Modified to make it compile
 *  CryptoLW-RK             https://github.com/rickkas7/CryptoLW-RK - 0.2.0
 *  HIH8000                 No Library, Local functions hih8_initialize(), hih8_getTempHumid() - rjb
 *  SENS0390                https://wiki.dfrobot.com/Ambient_Light_Sensor_0_200klx_SKU_SEN0390 - DFRobot_B_LUX_V30B - 1.0.1 I2C ADDRESS 0x94
 *  EEPROM                  https://docs.particle.io/reference/device-os/api/eeprom/eeprom/
 *                          On Gen 3 devices (Argon, Boron, B Series SoM, Tracker SoM, and E404X) 
 *                          the EEPROM emulation is stored as a file on the flash file system. 
 *                          Since the data is spread across a large number of flash sectors, 
 *                          flash erase-write cycle limits should not be an issue in general.
 * 
 *  Dallas OneWire          https://github.com/particle-iot/OneWireLibrary - Not supported on Boron/Argon/Muon
 *                          WARNING:This library has not been updated for the Argon, Boron, Tracker One, Photon 2/P2.
 *                          https://docs.particle.io/reference/device-os/libraries/o/OneWire/
 *
 *  LeafSens                https://github.com/tinovi/LeafArduino   I2C ADDRESS 0x61
 *  i2cArduino              https://github.com/tinovi/i2cArduinoI2c I2C ADDRESS 0x63
 *  i2cMultiSm              https://github.com/tinovi/i2cMultiSoilArduino/tree/master/lib ADDRESS 0x65
 * 
 *  AS5600                  Wind Direction - Bit Banged I2C ADDRESS 0x36
 *  AS5600L                 Wind Direction - Bit Banged I2C ADDRESS 0x40
 *  TMP112A                 Particle Muon on board temperature sensor - Bit Banged I2C ADDRESS 0x48
 * 
 * GPS Module - GNSS library for Particle SOMs - particle-som-gnss v1.0.0
 *   This standalone GNSS (Global Navigation Satellite System) library is designed specifically for Particle 
 *   cellular modems with built-in GNSS support, including the M-SOM platform.  
 *   It provides a simple and efficient interface to access and utilize GNSS data for location-based 
 *   applications on Particle devices.
 * 
 *   Adds location.h  Source at https://github.com/particle-iot/particle-som-gnss
 * 
 *  The GPS (GNSS) functionality on the Particle Muon will continue to work even if you run Cellular.off();,
 *  as the GNSS module operates independently from the cellular modem on supported Particle devices.
 *  It communicates internally over UART (Serial2 on pins D24/D25)
 *     D24 (Pin 16): Serial2 TX (UART transmit from MCU)
 *     D25 (Pin 18): Serial2 RX (UART receive to MCU)
 * 
 * LoRa Module
 *   Adafruit RFM95W LoRa Radio Transceiver Breakout - 868 or 915 MHz https://www.adafruit.com/product/3072
 * 
 *  LoRa Antenna Options
 *    915 MHz - 3 inches or 7.8 cm
 *    868 MHz - 3.25 inches or 8.2 cm
 *    433 MHz - 6.5 inches, or 16.5 cm
 *    https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/antenna-options
 * 
 * Distance Sensors
 * The 5-meter sensors (MB7360, MB7369, MB7380, and MB7389) use a scale factor of (Vcc/5120) per 1-mm.
 * Particle 12bit resolution (0-4095),  Sensor has a resolution of 0 - 5119mm,  Each unit of the 0-4095 resolution is 1.25mm
 * 
 * Tinovi Moisture Sensors
 * Non-Contact Capacitive leaf wetness, Temperature sensor
 *   https://tinovi.com/shop/i2c-non-contact-capacitive-leaf-wetness-temperature/
 *   https://tinovi.com/wp-content/uploads/2021/10/Leaf-Wetness-i2c-2021-10-11.pdf
 * 
 * PM-WCS-3-I2C I2C Non-Contact Capacitive Soil Moisture, Temperature sensor
 *   https://tinovi.com/shop/i2c-capacitive-soil-moisture-temperature-and-ec-sensor-variation-cable/
 *   https://tinovi.com/wp-content/uploads/2022/08/PM-WCS-3-I2C.pdf
 * 
 * SOIL-MULTI-5-I2C I2C Capacitive multi level soil moisture, temperature sensor
 *   https://tinovi.com/shop/soil-multi-5-i2c-i2c-capacitive-soil-moisture-temperature-sensor/
 *   https://tinovi.com/wp-content/uploads/2024/07/SOIL-MULTI-5-I2C.pdf
 *
 * Battery Charger Status from System.batteryState()
 *  0 = BATTERY_STATE_UNKNOWN
 *  1 = BATTERY_STATE_NOT_CHARGING
 *  2 = BATTERY_STATE_CHARGING
 *  3 = BATTERY_STATE_CHARGED
 *  4 = BATTERY_STATE_DISCHARGING
 *  5 = BATTERY_STATE_FAULT
 *  6 = BATTERY_STATE_DISCONNECTED
 *
 * Publish to Particle
 *  Event Name: FS
 *  Event Variables:
 *  at      timestamp
 *  rg      rain gauge 1
 *  rgt     rain gauge 1 total today
 *  rgp     rain gauge 1 total prior
 *  rg2     rain gauge 2
 *  rgt2    rain gauge 2 total today
 *  rgp2    rain gauge 2 total prior
 *  ws      wind speed
 *  wd      wind direction
 *  wg      wind gust
 *  wgd     wind gust direction
 *  bp1     bmx_pressure
 *  bt1     bmx_temperature
 *  bh1     bmx_humidity
 *  bp2     bmx_pressure
 *  bt2     bmx_temperature
 *  bh2     bmx_humidity
 *  mt1     mcp_temp MCP1
 *  mt2     mcp_temp MCP2
 *  gt1     globe_temp MCP3
 *  gt2     globe_temp MCP4
 *  hh1     htu_humidity
 *  ht1     htu_temperature
 *  st1     sht_temperature
 *  sh1     sht_humidity
 *  st2     sht_temperature
 *  sh2     sht_humidity
 *  hdt1    hdc_temperature
 *  hdh1    hdc_humidity
 *  hdt2    hdc_temperature
 *  hdh2    hdc_humidity
 *  ht2     hih_temperature
 *  hh2     hih_humidity
 *  lpp1    lps35hw_humidity
 *  lpt1    lps35hw_temperature
 *  lpp2    lps35hw_humidity
 *  lpt2    lps35hw_temperature
 *  sv1     si_visible
 *  si1     si_infrared
 *  su1     su_ultraviolet
 *  vlx     veml_lux
 *  blx     blux30_lux
 *  sg      snow / stream gauge
 *  pm1s10  Standard Particle PM1.0
 *  pm1s25  Standard Particle PM2.5
 *  pm1s100 Standard Particle PM10.0 
 *  pm1e10  Atmospheric Environmental PM1.0
 *  pm1e25  Atmospheric Environmental PM2.5
 *  pm1e100 Atmospheric Environmental PM10.0
 *  hi      heat index
 *  wbt     wet bulb temperature
 *  wbgt    wet bub globe temperature
 *  tlwt    Tinovi Leaf Wetness temperature    
 *  tlww    Tinovi Leaf Wetness
 *  tsmt    Tinovi Soil Moisture temperature
 *  tsme25  Tinovi Soil Moisture e25
 *  tsmec   Tinovi Soil Moisture ec
 *  tsmvwc  Tinovi Soil Moisture vwc
 * 
 *  MUX sensors where [1-8] is the MUX channel
 *  tsmt-[1-8]    Tinovi Soil Moisture temperature
 *  tsme25-[1-8]  Tinovi Soil Moisture e25
 *  tsmec-[1-8]   Tinovi Soil Moisture ec
 *  tsmvwc-[1-8]  Tinovi Soil Moisture vwc
 * 
 *  tmsmt   Tinovi Multi Level Soil Moisture
 *  tmsms1  Tinovi Multi Level Soil Moisture Soil Sensor 1 vwc
 *  tmsms2  Tinovi Multi Level Soil Moisture Soil Sensor 2 vwc
 *  tmsms3  Tinovi Multi Level Soil Moisture Soil Sensor 3 vwc
 *  tmsms4  Tinovi Multi Level Soil Moisture Soil Sensor 4 vwc
 *  tmsms5  Tinovi Multi Level Soil Moisture Soil Sensor 5 vwc
 * 
 * State of Health - Variables included with transmitted sensor readings
 *  bcs  = Battery Charger Status
 *  bpc  = Battery Percent Charge
 *  cfr    Charger Fault Register
 *  css  = Cell Signal Strength - percentage (0.0 - 100.0)
 *  hlth = Health 32bits - See System Status Bits in below define statements
 *  pmts = Particle Muon on board temperature sensor
 * 
 * AN002 Device Powerdown
 * https://support.particle.io/hc/en-us/articles/360044252554?input_string=how+to+handle+low+battery+and+recovery
 * 
 * NOTE: Compile Issues
 * If you have compile issues like multiple definations of functions then you need to clean the compile directory out
 *    ~/.particle/toolchains/deviceOS/2.0.1/build/target/user/...
 * 
 * Modified Libraries
 * In Adafruit_BMP280/src/Adafruit_BMP280.h Lines 39,40  Comment out the below 2 lines
 *   extern TwoWire Wire;
 *   extern SPIClass SPI;
 *   Uncommented code for takeForcedMeasurement()
 * In Adafruit_Sensor/src
 *   cp /dev/null Adafruit_Sensor.cpp
 * 
 * In Adafruit_HDC302x/src/Adafruit_HDC302x.cpp
 *   Read modified so while does not block
 *   int retries = 0;
 *   const int MAX_RETRIES = 10;
 *   while (!i2c_dev->read(buffer, 6)) {
 *     if (++retries >= MAX_RETRIES) {
 *       return false; // CRC check failed
 *     }
 *   }
 * 
 * In SdFat/src/common/ArduinoFiles.h 
 * #if defined(ARDUINO_SAM_DUE) && !defined(ARDUINO_API_VERSION)
 * void flush() { BaseFile::sync(); }
 * #else
 * // void flush() override { BaseFile::sync(); } <<<< Replace with below
 * void flush()  { BaseFile::sync(); }
 * #endi

 * DFRobot_B_LUX_V30B Library Not used it. It's bit banging with possible infinate loops - RJB
 * 
 * ========================================================
 * Particle Connection code based on below
 * ========================================================
 * https://community.particle.io/t/calling-particle-disconnect-after-a-failed-particle-connect-does-not-stop-led-from-blinking-green/19723/6
 * Note: Max Particle message size 622 characters 
 * 
 * ========================================================
 * Boron/Argon PIN Assignments
 * ========================================================
 * D8   = Serial Console (Ground Pin to Enable) - Not on Grove Shield
 * D7   = On Board LED - Lit when rain gauge tips, blinks when console connection needed
 * D6   = Reserved for Lora IRQ - Not on Grove Shield
 * D5   = SD Card Chip Select
 * D4   = SPI1 MSIO - Reserved for LoRa
 * D3   = SPI1 MOSI - Reserved for LoRa
 * D2   = SPI1 SCK  - Reserved for LoRa
 * D1   = I2C SCL
 * D0   = I2C SDA
 * 
 * A0   = WatchDog Monitor/Relay Reset Trigger
 * A1   = WatchDog Monitor Heartbeat
 * A2   = Wind Speed IRQ
 * A3   = Rain Gauge IRQ
 * A4   = Option 1 = Rain Gauge or Distance Gauge based on SD card file existing
 * A5   = Optional read/report of average on analog pin 
 * D13  = SPIO CLK   SD Card
 * D12  = SPI0 MOSI  SD Card
 * D11  = SPI0 MISO  SD Card
 * D10  = UART1 RX - Reserved for LoRa CS
 * D9   = UART1 TX - Reserved for LoRa RESET
 * 
 * ========================================================
 * Support for 3rd Party Sim Boron Only
 * ========================================================
 *   SEE https://support.particle.io/hc/en-us/articles/360039741113-Using-3rd-party-SIM-cards
 *   SEE https://docs.particle.io/cards/firmware/cellular/setcredentials/
 *   Logic
 *     Output how sim is configured (internal or external)
 *     If console is enabled and SD found and SIM.TXT exists at the top level of SD card
 *       Read 1st line from SIM.TXT. Parse line for one of the below patterns
 *        INTERNAL
 *        AUP epc.tmobile.com username passwd
 *        UP username password
 *        APN epc.tmobile.com
 *      Perform appropriate actions to set sim
 *      Rename file to SIMOLD.TXT, so we don't do this on next boot
 *      Output notification to user to reboot then flash board led forever
 *
 * ========================================================
 * Support for Argon WiFi Boards
 * ========================================================
 * SEE: https://docs.particle.io/reference/device-os/api/wifi/wifi/
 *
 * At the top level of the SD card make a file called WIFI.TXT
 * Add one line to the file
 * This line has 3 items that are comma separated Example
 * 
 * AuthType,ssid,password
 * 
 * Where AuthType is one of these keywords (WEP WPA WPA2 UNSEC)
 * Blank password is supported for UNSEC
 * 
 * ========================================================
 * MUON WiFi Enable
 * ========================================================
 * At the top level of the SD card make a file called WIFI.TXT
 * Add one line to the file
 * This line has 3 items that are comma separated Example
 * 
 * MUON,ssid,password
 * 
 * MUON is a keyword to distinguish from Argon WIFI.TXT file
 * 
 * ========================================================
 * Collecting Wind Data
 * ========================================================
 * Wind_SampleSpeed() - Return a wind speed based on how many interrupts and duration between calls to this function
 * Wind_SampleDirection() - Talk i2c to the AS5600 sensor and get direction
 * Wind_TakeReading() - Call this function every second. It calls wind direction and wind speed functions. Then saves samples in a circular buffer of 60 buckets.
 *
 * Creating the Wind Oobservations
 * Wind_DirectionVector() - Uses the 60 sample buckets of wind direction where speed is greater than zero to compute and return a wind vector.
 * Wind_SpeedAverage() - Use the 60 sample buckets of wind speed to return a wind speed average.
 * Wind_GustUpdate() - Uses the most current highest 3 consecutive samples of wind speed from the 60 samples. The 3 samples are then averaged for wind gust. 
 *   Variables wind.gust and wind.gust_directionare set by this function. 
 *   Call this function before calling Wind_Gust() and Wind_GustDirection()
 * Wind_Gust() - Returns wind.gust
 * Wind_GustDirection() - Returns wind.gust_direction
 * 
 * ========================================================
 * Setup for Air Quality Station
 * ========================================================
 * File OPTAQS.TXT - Enables Air Quality Station.
 * This feature then disable Wind, rain, and other OPT configs
 * Files OP1DIST.TXT, OP1RAIN.TXT, OP1RAW.TXT, OP2RAW.TXT are ignored
 * Pin OP2_PIN is wired to SET pin on sensor. The SET pin, when low, puts sensor into sleep mode.
 * The sleep mode on the Plantower PMSA003I is a feature that allows the sensor to enter a low-power state to conserve 
 *   energy and extend the lifetime of the laser and fan components. Reduces wear on moving parts, especially the fan 
 *   and laser, both of which have a finite lifespan (laser is about 8000 hours).
 * After waking the sensor by setting the SET pin high:
 *   The sensor resumes normal operation on the same I2C bus address.
 *   The Arduino library and communication remain established unless the microcontroller itself has reset or the sensor 
 *     was power-cycled (not just slept).
 *   It is still necessary to wait at least 30 seconds after waking before trusting the sensor’s data, 
 *     due to the required warm-up/stabilization period following wake from sleep mode.
 *   So after waiting 30 seconds:
 *     Take a first reading and discard it.
 *     Take a second reading and use that value for your application.
 *     This approach clears any stale or buffered data and helps avoid using outdated measurements from 
 *        the sensor's startup phase.
 * 
 * Files needed CONFIG.TXT OBI5M.TXT OPTAQS.TXT
 * ======================================================================================================================
 */

/* 
 *=======================================================================================================================
 * Local Includes
 *=======================================================================================================================
 */
#include "include/ssbits.h"
#include "include/qc.h"             // Quality Control Min and Max Sensor Values on Surface of the Earth
#include "include/support.h"        // Support Functions
#include "include/sdcard.h"         // SD Card Functions
#include "include/cf.h"             // Configuration File Variables
#include "include/eeprom.h"         // EEPROM Functions
#include "include/lora.h"           // LoRa Functions    
#include "include/output.h"         // Serial and OLED Output Functions  
#include "include/wrda.h"           // Wind Rain Distance Air Functions
#include "include/mux.h"            // Mux Functions for mux connected sensors
#include "include/time.h"           // Time Management Functions
#include "include/ps.h"             // Particle Support Functions
#include "include/sensors.h"        // I2C Based Sensor Functions
#include "include/evt.h"           // Evapotranspiration Functions
#include "include/info.h"           // Station Information Functions
#include "include/statmon.h"        // Station Monitor Functions
#include "include/obs.h"            // Observation Functions
#include "include/main.h"

/*
 * ======================================================================================================================
 * Variables and Data Structures 
 * =======================================================================================================================
 */
char versioninfo[sizeof(VERSION_INFO)];  // allocate enough space including null terminator
char msgbuf[MAX_MSGBUF_SIZE]; // Used to hold messages
char *msgp;                   // Pointer to message text
char Buffer32Bytes[32];       // General storage
#if (PLATFORM_ID == PLATFORM_MSOM)
int LED_PIN = D22;             // Added LED Header Pin 22
bool MuonWifiEnabled = false;  // Set if we find a WIFI.TXT file
#else
int  LED_PIN = D7;            // Built in LED
#endif
bool TurnLedOff = false;      // Set true in rain gauge interrupt
bool JustPoweredOn = true;    // Used to clear SystemStatusBits set during power on device discovery
bool SendSystemInformation = true; // Send System Information to Particle Cloud. True means we will send at boot.

uint64_t lastOBS = 0;         // time of next observation
int countdown = 600;          // Exit station monitor/mode - when countdown reaches 0
                              // Protects against burnt out pin or forgotten jumper

uint64_t LastTimeUpdate = 0;
uint64_t LastTransmitTime = 0;

int  cf_reboot_countdown_timer = 79200; // There is overhead transmitting data so take off 2 hours from 86400s
                                        // Set to 0 to disable feature
int DailyRebootCountDownTimer;

uint64_t obs_interval = DEFAULT_OBS_INTERVAL;  // Default OBS interval 1 Minute
uint64_t obs_tx_interval = DEFAULT_OBS_TRANSMIT_INTERVAL;  // Default OBS Transmit interval 15 Minutes


#if (PLATFORM_ID == PLATFORM_BORON) || (PLATFORM_ID == PLATFORM_MSOM)
PMIC pmic; // Power Management IC (bq24195) I2C 0x6B
#else
// FuelGauge fuel;  // Fuel Gauge IC (MAX17043) I2C 0x36
#endif


/*
 * ======================================================================================================================
 * HeartBeat() - Burns 250 ms and part of our loop delay timing
 * ======================================================================================================================
 */
void HeartBeat() {
#if (PLATFORM_ID == PLATFORM_MSOM)
  uint64_t MsFromNow = System.millis() + 250;
  Watchdog.refresh();
  delay((int64_t)(MsFromNow - System.millis()));
#else
  digitalWrite(HEARTBEAT_PIN, HIGH);
  delay(250);
  digitalWrite(HEARTBEAT_PIN, LOW);
#endif
}

/*
 * ======================================================================================================================
 * BackGroundWork() - Take Sensor Reading, Check LoRa for Messages, Delay 1 Second for use as timming delay            
 * ======================================================================================================================
 */
void BackGroundWork() {
  // Anything that needs sampling every second add below. Example Wind Speed and Direction, StreamGauge

  uint64_t OneSecondFromNow = System.millis() + 1000;

  if (!AQS_Enabled) {
    Wind_TakeReading();

    if (OP1_State == OP1_STATE_DISTANCE) {
      DistanceGauge_TakeReading();
    }

    if (PM25AQI_exists) {
      pm25aqi_TakeReading();
    }
  }

  HeartBeat();  // Provides a 250ms delay

  if (LORA_exists) {
    lora_msg_poll(); // Provides a 750ms delay
  }

  int64_t TimeRemaining = (OneSecondFromNow - System.millis());
  if ((TimeRemaining > 0) && (TimeRemaining < 1000)) {
    delay (TimeRemaining);
  }

  if (!AQS_Enabled) {
    if (TurnLedOff) {   // Turned on by rain gauge interrupt handler
      digitalWrite(LED_PIN, LOW);  
      TurnLedOff = false;
    }
  }
}

// You must use SEMI_AUTOMATIC or MANUAL mode so the battery is properly reconnected on
// power-up. If you use AUTOMATIC, you may be unable to connect to the cloud, especially
// on a 2G/3G device without the battery.
SYSTEM_MODE(SEMI_AUTOMATIC);

// https://docs.particle.io/cards/firmware/system-thread/system-threading-behavior/
SYSTEM_THREAD(ENABLED);

/*
 * ======================================================================================================================
 * setup() - runs once, when the device is first turned on.
 * ======================================================================================================================
 */
void setup() {

#if (PLATFORM_ID == PLATFORM_MSOM)
  //  https://docs.particle.io/reference/datasheets/m-series/muon-datasheet/#firmware-settings

  // Retrive the current system power configuration object so it can be modified.
  SystemPowerConfiguration powerConfig = System.getPowerConfiguration();

  // This enables the feature related to PMIC detection. 
  powerConfig.feature(SystemPowerFeature::PMIC_DETECTION)
      .auxiliaryPowerControlPin(D7)  // sets the GPIO pin D7 as the auxiliary power control pin aka 3.3V
      .interruptPin(A7); // detect changes from the power management IC or related events
  System.setPowerConfiguration(powerConfig);

  // Enable 3.3V on GPIO Header from my code. Avoid the System.setPowerConfiguration.
  pinMode(D7, OUTPUT);
  digitalWrite(D7, 1);
#endif

  // The device has booted, reconnect the battery.
#if (PLATFORM_ID == PLATFORM_BORON) || (PLATFORM_ID == PLATFORM_MSOM)
	pmic.enableBATFET();
#endif

  // Set Default Time Format
  Time.setFormat(TIME_FORMAT_ISO8601_FULL);

#if (PLATFORM_ID == PLATFORM_MSOM)
  Watchdog.init(WatchdogConfiguration().timeout(120s));
  Watchdog.start();
#else
  pinMode (HEARTBEAT_PIN, OUTPUT);
#endif

  pinMode (REBOOT_PIN, OUTPUT);
  pinMode (LED_PIN, OUTPUT);
  
  Output_Initialize(); // Waits for Serial if Jumper in place for 60s
  delay(2000); // Prevents usb driver crash on startup, Arduino needed this so keeping for Particle

  Serial_write(COPYRIGHT);
  strcpy(versioninfo, VERSION_INFO);
  Output (versioninfo);

  System.enableFeature(FEATURE_RESET_INFO); 
  OutputResetReason();

  delay(2000); // Give some time to see this

  HeartBeat(); // Lets refresh Watchdog - just because we can

  // Set Daily Reboot Timer
  DailyRebootCountDownTimer = cf_reboot_countdown_timer;

  // Initialize SD card if we have one.
  SD_initialize();

  // Report if we have Need to Send Observations
  if (SD_exists && SD.exists(SD_n2s_file)) {
    SystemStatusBits |= SSB_N2S; // Turn on Bit
    Output("N2S:Exists");
  }
  else {
    SystemStatusBits &= ~SSB_N2S; // Turn Off Bit
    Output("N2S:None");
  }

  // Rename A4 and A5 files used in releases prior to release 40. 
  // Remove function this when we determine all sites are at release 40 or greater
  SD_A4A5_Rename();

  // If config file exists it is opened and read
  SD_ReadConfigFile();

  // If elevation file exists it is opened, read and elevation set
  SD_ReadElevationFile();

  // Display EEPROM Information 
  EEPROM_Dump();

  // Check if correct time has been maintained by RTC
  // Uninitialized clock would be 2000-01-00T00:00:00
  stc_timestamp();
  sprintf (msgbuf, "%sS", timestamp);
  Output(msgbuf);

  // Read RTC and set system clock if RTC clock valid 
  rtc_initialize();

  stc_timestamp();
  sprintf (msgbuf, "%sS", timestamp);
  Output(msgbuf);

  if (Time.isValid()) {
    Output("STC:VALID");
  }
  else {
    Output("STC:!VALID");
  }

  stc_timestamp();
  sprintf (msgbuf, "%sS", timestamp);
  Output(msgbuf);

#if (PLATFORM_ID == PLATFORM_BORON) || (PLATFORM_ID == PLATFORM_MSOM)
  // System Power and Battery State
  sprintf (msgbuf, "PS:%d", System.powerSource());
  Output(msgbuf);
  sprintf (msgbuf, "BS:%d", System.batteryState());
  Output(msgbuf);
  float bpc = System.batteryCharge();
  sprintf (msgbuf, "BPC:%d.%02d", (int)bpc, (int)(bpc*100)%100);
  Output(msgbuf);
#endif

#if (PLATFORM_ID == PLATFORM_MSOM)
  network_initialize();
  WiFiPrintCredentials();

  // Turn on GPS
  Output("GPS:Enable");
  LocationConfiguration config;
  config.enableAntennaPower(GNSS_ANT_PWR);
  Location.begin(config);
#endif

#if (PLATFORM_ID == PLATFORM_ARGON)
	pinMode(PWR, INPUT);
	pinMode(CHG, INPUT);
  //==================================================
  // Check if we need to program for WiFi change
  //==================================================
  WiFiPrintCredentials();
  WiFiChangeCheck();
  WiFiPrintCredentials();
#endif

#if (PLATFORM_ID == PLATFORM_BORON)
  //==================================================
  // Check if we need to program for Sim change
  //==================================================
  SimChangeCheck();
#endif

  // Check SD Card for file to determine if we are a Air Quality Station
  OBI_AQS_Initialize(); // Sets AQS_Enabled to true


  //==================================================
  // Wind Speed and Rain Gauge Interrupt Based Sensors
  //==================================================

  if (!AQS_Enabled) {
    // Optipolar Hall Effect Sensor SS451A - Wind Speed
    anemometer_interrupt_count = 0;
    anemometer_interrupt_stime = System.millis();
    attachInterrupt(ANEMOMETER_IRQ_PIN, anemometer_interrupt_handler, FALLING);

    // Optipolar Hall Effect Sensor SS451A - Rain Gauge
    raingauge1_interrupt_count = 0;
    raingauge1_interrupt_stime = System.millis();
    raingauge1_interrupt_ltime = 0;  // used to debounce the tip
    attachInterrupt(RAINGAUGE1_IRQ_PIN, raingauge1_interrupt_handler, FALLING);
  }

  // Check SD Card for files to determine Observation and Transmit Intervals
  OBI_TXI_Initialize();
  
  if (!AQS_Enabled) {
    // Check SD Card for files to determine if pin OP1 has a DIST, 2nd Rain Gauge or Raw file
    OP1_Initialize();

    // Check SD Card for files to determine if pin OP2 Raw file
    OP2_Initialize();
  }

#if (PLATFORM_ID == PLATFORM_MSOM)
  pmts_initialize();  // Particle Muon on board temperature sensor (TMP112A)
#endif



  // Scan for i2c Devices and Sensors
  mux_initialize();
  if (!MUX_exists) {
    tsm_initialize(); // Check main bus
  }
  bmx_initialize();
#if (PLATFORM_ID != PLATFORM_MSOM)
  htu21d_initialize();  // This sensor has same i2c address as AS5600L
#endif
  mcp9808_initialize();
  sht_initialize();
  hih8_initialize();
  si1145_initialize();
  vlx_initialize();
  blx_initialize();
  as5600_initialize();
  pm25aqi_initialize();
  hdc_initialize();
  lps_initialize();

  // Tinovi Mositure Sensors
#if (PLATFORM_ID != PLATFORM_MSOM)
  tlw_initialize();
#endif
  tmsm_initialize();
  
  // Derived Observations
  wbt_initialize();
  hi_initialize();
  wbgt_initialize();
  mslp_initialize();

#ifdef ENABLE_Evapotranspiration
  evt_initialize();   // checks for shortwave radiation from pyranometer via 16bit A/D
                      // Requires SHT_1_exists & AS5600_exists to be true for Evapotranspiration to run 
#endif
  
  // Initialize RH_RF95 LoRa Module
  lora_initialize();
  lora_device_initialize();
  lora_msg_check();

  // Connect the device to the Cloud. 
  // This will automatically activate the cellular connection and attempt to connect 
  // to the Particle cloud if the device is not already connected to the cloud.
  // Upon connection to cloud, time is synced, aka Particle.syncTime()

  // Note if we call Particle.connect() and are not truely connected to the Cell network, Code blocks in particle call
  Particle.setDisconnectOptions(CloudDisconnectOptions().graceful(true).timeout(5s));
  Particle.connect();
  
  // Setup Remote Function to DoAction, Expects a parameter to be passed from Particle to control what action
  if (Particle.function("DoAction", Function_DoAction)) {
    Output ("DoAction:OK");
  }
  else {
    Output ("DoAction:ERR");
  }

#if (PLATFORM_ID == PLATFORM_BORON) || (PLATFORM_ID == PLATFORM_MSOM)
  // Get International Mobile Subscriber Identity
  if ((RESP_OK == Cellular.command(callback_imsi, imsi, 10000, "AT+CIMI\r\n")) && (strcmp(imsi,"") != 0)) {
    sprintf (msgbuf, "IMSI:%s", imsi);
    Output (msgbuf);
  }
  else {
    Output("IMSI:NF");
  }
#endif

  if (Time.isValid()) {
    // We now a a valid clock so we can initialize the EEPROM and make an observation
    EEPROM_Initialize();     
  }
  // Lets force a publish if not doing 1 minute observations
  if (obs_interval != DEFAULT_OBS_INTERVAL) {
    lastOBS = 0;
    LastTransmitTime = 0; 
  }
  ws_refresh = true;
}

/*
 * ======================================================================================================================
 * loop() runs over and over again, as quickly as it can execute.
 * ======================================================================================================================
 */
void loop() {
  // If Serial Console Pin LOW then Display Station Information
  // if (0 && countdown && digitalRead(SCE_PIN) == LOW) {     // !!!!!!!!!!!! Remove the 0
  if (countdown && digitalRead(SCE_PIN) == LOW) {     
    StationMonitor();
    BackGroundWork();
    countdown--;
  }
  else { // Normal Operation - Main Work

    // This will be invalid if the RTC was bad at poweron and we have not connected to Cell network
    // Upon connection to cell network system Time is set and this becomes valid
    if (Time.isValid()) {  
 
      // Set RTC from Cell network time.
      RTC_UpdateCheck();

      if (!eeprom_valid) {
        // We now a a valid clock so we can initialize the EEPROM
        EEPROM_Initialize();
      }

      if (SendSystemInformation && Particle.connected()) {
        INFO_Do(); // Function sets SendSystemInformation back to false.
      }

      // If we waited too long for acks while publishing and this threw off our wind observations.
      // In that code ws_refresh was set to true for us to reinit wind data.
      if (!AQS_Enabled) {
        if (ws_refresh) {
          Output ("WS Refresh Required");
          Wind_Distance_Air_Initialize();
        }
      }

      // Perform an Observation, save in OBS structure, Write to SD
      if ( (lastOBS == 0) || (System.millis() - lastOBS) > ((obs_interval*60*1000)-AQS_Correction) ) {
        OBS_Do();
      }

      // Time to Send Observations we have collected
      if ( (LastTransmitTime == 0) || ((System.millis() - LastTransmitTime) > (obs_tx_interval * 60 * 1000)) ) {
        if (Particle.connected()) {
          Output ("Connected");
          LastTransmitTime = System.millis();

          // Incase we are staying connected to the Cell network
          // request time synchronization from the Cell network - Every 4 Hours
          if ((System.millis() - LastTimeUpdate) > (4*3600*1000)) {
            // Note that this function sends a request message to the Cloud and then returns. 
            // The time on the device will not be synchronized until some milliseconds later when 
            // the Cloud responds with the current time between calls to your loop.

            // !!! What if we drop the Cell connection before we get a time update for the Cloud?
            //     If we have 15 observations to send, that 15s + 5s graceful particle disconnect
            Particle.syncTime();
            LastTimeUpdate = System.millis();
          }

          OBS_PublishAll();

          // Update OLED and Console
          stc_timestamp();
          Output(timestamp);
          Output_CellBatteryInfo();

          // Shutoff System Status Bits related to initialization after we have logged first observation 
          JPO_ClearBits();
        }
      }

#ifdef ENABLE_Evapotranspiration
      // Samples once a minute and reports on the hour 
      evt_do();
#endif

#if PLATFORM_ID == PLATFORM_ARGON
      // See if it's been an hour without a network connection and transmission of data
      // With Argon WiFi we have seen it stuck in Breathing Green - Trying to connect
      if (System.millis() - LastTransmitTime > (3600 * 1000)) {  
        // Been too long with out a network connection, lets reboot
        Output("1HR W/O NW: Rebooting");
        delay(5000);
        System.reset();
      }
#endif
      // Do background work, delays 1 Second
      BackGroundWork();
    }
    else {

      stc_timestamp();
      Output(timestamp);
      Output("ERR: No Clock");
      delay (DELAY_NO_RTC);
    }

    // ========================================================================================
    // Reboot Boot Every 22+ hours - Not using time but a loop counter.
    // ========================================================================================
    if ((cf_reboot_countdown_timer>0) && (--DailyRebootCountDownTimer<=0)) {
      Output ("Daily Reboot");

      if (Particle.connected()) {
        OBS_PublishAll();
      }

      EEPROM_SaveUnreportedRain();
      delay(1000);

      // Lets not rip the rug out from the modem. Do a graceful shutdown.
      Particle.disconnect();
      waitFor(Particle.disconnected, 1000);  // Returns true when disconnected from the Cloud.

#if (PLATFORM_ID == PLATFORM_BORON) || (PLATFORM_ID == PLATFORM_MSOM)
      // Be kind to the cell modem and try to shut it down
      Cellular.disconnect();
      delay(1000);
      Cellular.off();
#endif

      Output("Rebooting");  
      delay(1000);
   
      DeviceReset();

      // We should never get here, but just incase 
      Output("I'm Alive! Why?");  

#if (PLATFORM_ID == PLATFORM_BORON) || (PLATFORM_ID == PLATFORM_MSOM)
		  Cellular.on();
      delay(1000);
#endif

		  Particle.connect();

      DailyRebootCountDownTimer = cf_reboot_countdown_timer; // Reset count incase reboot fails

      // We need to reinitialize our wind readings before we can move on.
      if (!AQS_Enabled) {
        Wind_Distance_Air_Initialize();
      }
    }   

#if (PLATFORM_ID == PLATFORM_BORON) || (PLATFORM_ID == PLATFORM_MSOM)
    // ========================================================================================
    // Low Power Check and Power Off
    // ========================================================================================

    // Before we go do an observation and transmit, check our power status.
    // If we are not connected to a charging source and our battery is at a low level
    // then power down the display and board. Wait for power to return.
    // Do this at a high enough battery level to avoid the board from powering
    // itself down out of our control. Also when power returns to be able to charge
    // the battery and transmit with out current drops causing the board to reset or 
    // power down out of our control.

    if ((System.powerSource() == POWER_SOURCE_BATTERY) && (System.batteryCharge() <= 15.0)) {
      Output("Low Power!");

      if (Particle.connected()) {
        OBS_PublishAll(); 
        INFO_Do();
      }

      // While this function will disconnect from the Cloud, it will keep the connection to the network.
      Particle.disconnect();
      waitFor(Particle.disconnected, 1000);  // Returns true when disconnected from the Cloud.
      
      Cellular.disconnect();
      delay(1000);
      Cellular.off();
       
      if (LORA_exists) {
        rf95.sleep(); // Power Down LoRa. Note: it turn on when accessed
      }

      Output("Powering Down");

      OLED_sleepDisplay();
      delay(5000);

      // Disabling the BATFET disconnects the battery from the PMIC. Since there
		  // is no longer external power, this will turn off the device.
		  pmic.disableBATFET();

		  // This line should not be reached. When power is applied again, the device
		  // will cold boot starting with setup().

		  // However, there is a potential for power to be re-applied while we were in
		  // the process of shutting down so if we're still running, enable the BATFET
		  // again and reconnect to the cloud. Wait a bit before doing this so the
		  // device has time to actually power off.
		  delay(2000);

      OLED_wakeDisplay();   // May need to toggle the Display reset pin.
		  delay(2000);
		  Output("Power Re-applied");

      // WakeUp LoRa
      // May need to toggle LoRa Reset

		  pmic.enableBATFET();

		  Cellular.on();

		  Particle.connect();

      // We need to reinitialize our wind readings before we can move on.
      if (!AQS_Enabled) {
        Wind_Distance_Air_Initialize();
      }
    }
#endif
  }
}
