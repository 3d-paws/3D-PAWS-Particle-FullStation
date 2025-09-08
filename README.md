# 3D-PAWS Particle Full Station

Last Updated: 2025-09-01

## Description

This software is supported on a Particle Boron, Argon band Muon boards. It provides full weather station features. Support a Air Quality Sensor mode. This document supports Version 40 and aftere. See [README_v39.md](docs/README_v39.md) for prior releases.
## Features

### [Serial Monitor](docs/SerialMonitor.md)
Adding a jumper wire between Particle pin D8 (Boron & Argon), A2 (Muon) and ground will enable serial text output on the USB connector at boot time.

### [OLED Display](docs/OLED_Display.md)
If an OLED display is detected at startup, messages are also sent to this display. Jumper wire is not needed for activation.

### [Station Monitor](docs/StationMonitor.md)
Upon setup completion.  If the jumper wire is in place then the Station Monitor will run instead of the main work loop.  

### [Boron Board Support](docs/Boron.md)
Particle Cellular Board Support.

### [Argon Board Support](docs/Argon.md)
Particle WiFi Board Support.

### [Muon Board Support](docs/Muon.md)
Particle Muon Board Support.

### [Particle Console](docs/ParticleConsole.md)
Features and Setup of the Particle Web Console

### [SD Card Information](docs/SD.md)
Information on what is stored on the SD card.

### [LoRa Remote Relay](docs/LoRaRelay.md)
Receive LoRa messages for LoRa Remote devices. Transmit the message to Particle next transmission period.

### [Sensor Information](docs/Sensors.md)
Information about supported Sensors

### [Station Information](docs/INFO.md)
Information about Particle message type INFO and INFO.TXT file.

### [Air Quality Station Option and Operation](docs/AirQualityStation.md)
Information about operation as a Air Quality Station (No wind, Rain) and sampling air quality differently.

### [Code Operation Notes](docs/CodeOperation.md)
Information on how the station operates at a code level.

### WatchDog Board
 

### Battery Charger Status (bcs)
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 140px; border: 1px solid black; padding: 10px;">
<pre>
0 = BATTERY_STATE_UNKNOWN
1 = BATTERY_STATE_NOT_CHARGING
2 = BATTERY_STATE_CHARGING
3 = BATTERY_STATE_CHARGED
4 = BATTERY_STATE_DISCHARGING
5 = BATTERY_STATE_FAULT
6 = BATTERY_STATE_DISCONNECTED

Battery Percent Charge (bpc)
Cell Signal Strength (css)
</pre>
</div>

### Transmitted Health Information (hth)
Many of the below bits will be set at initialization. Then cleared after the first observation is made after startup. If a sensor existed then disappeared, a bit will be set until the sensor returns or a reboot occurs.
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 350px; border: 1px solid black; padding: 10px;">
<pre>
Health bits (hth)
AREA         BIT       COMMENT
OKAY         0x0       Normal Operation
PWRON        0x1       Set at power on, but cleared after first observation
SD           0x2       Set if SD missing at boot or other SD related issues
RTC          0x4       Set if RTC missing at boot
OLED         0x8       Set if OLED missing at boot, but cleared after first observation
N2S          0x10      Set when Need to Send observations exist
FROM_N2S     0x20      Set in transmitted N2S observation when finally transmitted
AS5600       0x40      Set if wind direction sensor AS5600 has issues
BMX_1        0x80      Set if Barometric Pressure & Altitude Sensor missing
BMX_2        0x100     Set if Barometric Pressure & Altitude Sensor missing
HTU21DF      0x200     Set if Humidity & Temp Sensor missing
SI1145       0x400     Set if UV index & IR & Visible Sensor missing
MCP_1        0x800     Set if Precision I2C Temperature Sensor missing
MCP_2        0x1000    Set if Precision I2C Temperature Sensor missing
MCP_3        0x2000    Set if Precision I2C Temperature Sensor missing
LORA         0x4000    Set if LoRa Radio missing at startup
SHT_1        0x8000    Set if SHTX1 Sensor missing
SHT_2        0x10000   Set if SHTX2 Sensor missing
HIH8         0x20000   Set if HIH8000 Sensor missing
VLX          0x40000   Set if VEML7700 Sensor missing
PM25AQI      0x80000   Set if PM25AQI Sensor missing
HDC_1        0x100000  Set if HDC302x Sensor missing
HDC_2        0x200000  Set if HDC302x Sensor missing
BLX          0x400000  Set if BLUX30 I2C Sensor missing
LPS_1        0x800000  Set if LPS35HW I2C Sensor missing
LPS_2        0x1000000 Set if LPS35HW I2C Sensor missing
SSB_TLW      0x2000000 Set if Tinovi Leaf Wetness I2C Sensor missing
SSB_TSM      0x4000000 Set if Tinovi Soil Moisture I2C Sensor missing
SSB_TMSM     0x8000000 Set if Tinovi MultiLevel Soil Moisture I2C Sensor missing
</pre>
</div>
Interpreting health bits in relation to Need to Send observations.
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 100px; border: 1px solid black; padding: 10px;">
<pre>
 0 = All is well, no data needing to be sent, this observation is not from the N2S file
16 = There is N2S data, This observation is not from the N2S file
32 = This observation is from the N2S file. And when it was saved to the N2S file, the N2S file
        did not exist. So it is the first observation from the file.
48 = This observation is from the N2S file. And when it was saved to the N2S file, the N2S file
        existed and this observation was appended.
</pre>
</div>


