# Configuration File Example
[←Top](../README.md)<BR>


Below is an example of CONFIG.TXT located at the top directory on the SD Card. What is shown below is for Version 49 and after. This file is rewritten by the software when a configuration is changed via doaction. All comments will be lost in this process.

CONFIG.TXT
```
# Valid entries are 0=Full Station, 1=AirQuality Station
aqs=0

# Valid entries are 0=Disabled, 1=Enabled
wind=1

# Valid entries are 0=Disabled, 1=Enabled
rg1=1

# Option Pin 1. Valid entries are  0=Null, 1=Distance, 2=Rain (rg2), 3=Raw
op1=0

# Option Pin 1 Distance sensor type. Valid entries are 0=10m Distance, 1=5m Distance Sensor
op1d5m=0

# Option Pin 2. Valid entries are 0=Null, 1=Raw, 2=Voltaic
op2=0

# Rain Total RollOver Hour. Valid entries are 0-23 
rtro_hour=0

# Rain Total Rollover Minute. Valid entries are 0,15,30,45
rtro_minute=0

# Station Elevation in m, needed for MSLP
elevation=0    

lat_deg=0.0
lon_deg=0.0

# Reboot CountDown Timer ~ every 22hrs
rcdt=79200

# Observation transmit interval 15m
txi=15

# Observation Intervam 1m
obi=1

# Private Key - 128 bits (16 bytes of ASCII characters)
aes_pkey=10FE2D3C4B5A6978

# Initialization Vector must be and always will be 128 bits (16 bytes.)
# The real iv is actually myiv repeated twice
# 1234567 -> 0x12D687 = 0x00 0x12 0xD6 0x87 0x00 0x12 0xD6 0x87 
aes_myiv=1234567

# This unit's LoRa ID for Receiving and Sending Messages
lora_unitid=1

# Valid entries are 5 to 23 dBm
lora_txpower=5

# Valid entries are 433, 866, 915
lora_freq=915

# Valid entries are WEP, WPA, WPA2, UNSEC; Not used for MUON
wifi_auth=
wifi_ssid=
wifi_pw=

//Evapotranspiration
albedo=0.23f
crop_kc=1.0f
sr_cal=75.0f
sr_dark_offset=0.0f

```

Below is the configuration file pror to Version 49. This file and the various .TXT files will be used when running older versions. When Version 49 is installed. The configuration file is read and if entry "aqs" is not found in the file; a configuration migration takes place. The .TXT files are read in and a new CONFIG.TXT file is written to the SD card. Then the same file is written to non volatile memory.  After which, if the SD card is not detected, the non volatile config is used.
```
#
# CONFIG.TXT
#
# Line Length is limited to 63 characters
#12345678901234567890123456789012345678901234567890123456789012

# Private Key - 128 bits (16 bytes of ASCII characters)
aes_pkey=10FE2D3C4B5A6978

# Initialization Vector must be and always will be 128 bits (16 bytes.)
# The real iv is actually myiv repeated twice
# 1234567 -> 0x12D687 = 0x00 0x12 0xD6 0x87 0x00 0x12 0xD6 0x87 
aes_myiv=1234567

# This unit's LoRa ID for Receiving and Sending Messages
lora_unitid=1

# You can set transmitter power from 5 to 23 dBm
lora_txpower=5

# Valid entries are 433, 866, 915
lora_freq=915

########################################
# Evapotranspiration - evt.cpp and evt.h
########################################
lat_deg=40.02
lon_deg=-105.26
# Set elevation (m) in file ELEV.TXT - Used for MSLP and setable via Particle console DoAction

# Value 0.23 (grass)
albedo=0.23

# Value 1.0 sets to baseline crop grass 
crop_kc=1.0

# Irradiance
# Replace with your Irradiance sensor's calibration constant (µV per W/m² from the label/certificate)
# Check the calibration sticker on your SP Lite2 — it will say something like: Sensitivity: 74.8 µV per W/m²
sr_cal=75.0 

# With the Irradiance sensor covered, log the raw readings, set that as dark offset.
sr_dark_offset=0.0
```