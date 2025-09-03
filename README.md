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

### [Boron Board Support](docs/Argon.md)
Particle Cellular Board Support.

### [Argon Board Support](docs/Argon.md)
Particle WiFi Board Support.

### [Muon Board Support](docs/Muon.md)
Particle Muon Board Support.

### [Particle Console Do Action Functions](docs/DoAction.md)
From Particle Consolde send commands to boards.


### SD card Information

Size: 32 Gig or smaller cards. Larger SD cards will not be seen.  

Format Tool: SD Card Formatter by TUXERA <https://www.sdcard.org/downloads/formatter/>  

Format: MS-DOS (FAT32) Format  

Files and Directories:
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 200px; border: 1px solid black; padding: 10px;">
<pre>
  /OBS              Directory containing observation files.  
  /OBS/20231024.LOG Dated observation file. One file per day. Data in JSON format  
  /N2SOBS.TXT       Need to Send file. Stores observations that have not been
                    transmitted. If file is greater than 512 *60* 48 bytes.
                    File is deleted and we start over. File is deleted when all
                    N2S observations have been sent.  
  /A4RAIN.TXT   Set pin A4 to be a 2nd rain gauge.
  /A4RAW.TXT    set pin A4 for raw readings (Average of 5 samples 10ms apart).
  /A4DIST.TXT   Set pin A4 to be a distance gauge of type 10m.
  /5MDIST.TXT   Set distance gauge of type 5m.
  /SIM.TXT      Support 3rd party SIM.
  /A5RAW.TXT    set pin A5 for raw readings (Average of 5 samples 10ms apart).  
  /WIFI.TXT     Support for Argon WiFi Boards. Stores WiFi information.
  /INFO.TXT     INFO feature information. Every INFO call will overwrite files content.
  /TXI5M.TXT    Set Transmit Interval to 5 minutes.
  /TXI10M.TXT   Set Transmit Interval to 10 minutes.
  No TXIxxM.TXT Set Transmit Interval to 15 minutes.
</pre>
</div>

## Code Operation

### Timing
Every minute software takes an observation and stores it to the SD card in a dated file. Which is located under the /OBS folder.

Every second, Wind Speed & Direction occurs. If sensors for Air Particle and Distance are attached, these are also sampled.

Within the 1 second window Particle pin A1 is toggled on for 250ms. Providing the board's heartbeat.  If LoRa is activated, inbound LoRa messages are checked for multiple times.

Every 15 minutes observations are transmitted to the Particle Cloud.  The 15 minute time of transmit is based on the first occurrence of having a network connection after boot. Not having a predetermined 15 minute window, time spreads out when we transmit over the 15 minute period. This helps not overload the receiving site when large numbers of stations are reporting.  

### Collecting Wind Data
Wind_SampleSpeed() - Return a wind speed based on how many interrupts and duration between calls to this function.
Wind_SampleDirection() - Talk i2c to the AS5600 sensor and get direction.
Wind_TakeReading() - Call this function every second. It calls wind direction and wind speed functions. Then saves samples in a circular buffer of 60 buckets.

### Creating the Wind Observations
Wind_DirectionVector() - Uses the 60 sample buckets of wind direction where wind speed is greater than zero to compute and return a wind vector.
Wind_SpeedAverage() - Uses the 60 sample buckets of wind speed to return a wind speed average.
Wind_GustUpdate() - Uses the most current highest 3 consecutive samples of wind speed from the 60 samples. The 3 samples are then averaged for wind gust.
Variables wind.gust and wind.gust_directionare set by this function.
Call this function before calling Wind_Gust() and Wind_GustDirection().
Wind_Gust() - Returns wind.gust
Wind_GustDirection() - Returns wind.gust_direction

### Transmitting Observations and Failure Handling
One minute observations are sent to Particle Cloud with Particle Event Name "FS". This is used by the Particle Cloud service to identify and associate webhooks for triggering.

After transmitting an observation, software will wait for a confirmation result code from Particle Cloud. If the result code reports failure or we didn't have network connectivity at the time of transmission. The observation is appended to the Need to Send file located on the SD card at the top level and called N2SOBS.TXT. If the file does not exist, it is created then appended to. Observations in this file will later be transmitted.

Caveat: At the time of appending the observation to the N2S file, if the file is greater than 512 *60* 48 bytes. ~2 days of observations. The file is deleted. Recreated and the observation is appended.

### Sending N2S Observations
After successful completion of current 1 minute observations have finished transmitting. If N2S observations exist, these are then read from the file and transmitted.  Position in the N2S file of what has been transmitted is maintained in nvram. So rebooting does not cause the retransmission of N2S observations. On a N2S observation transmit failure, N2S processing stops until the next 15 minute window.  While sending N2S observations, 1 second tasks and 1 minute observations still occur. When storage space for 1 minute observations becomes full. N2S processing stops and the main work loop takes over.  At which point current 1 minute observations will be transmitted. Freeing up the observation storage.

### LoRa Observations
When LoRa is enabled, the full station will receive LoRa messages from remote LoRa stations, store the observations in memory and transmit them at the 15 minute window. For remote LoRa stations see 3D-PAWS-Feather-LoRaRemoteM0 code base.

### Rain and Soil Moisture

After the Full Station 1 minute observations are sent at the 15 minute window. LoRa observations types of (INFO, RS, SG) which were received and stored since the last 15 minute transmit will be sent.

RS observations are sent to Particle Cloud with Particle Event Name "RS".

The Full Station can support receiving and storing 32 LoRa messages in memory. They will be added to the N2S file when necessary.

### Example of Particle Webhook for Full Station Observations (FS)

<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 200px; border: 1px solid black; padding: 10px;">
<pre>

Observation Time (at)
Chords ID (id)
Battery Voltage bv
Health (hth)
Rain Gauge (rg)
Soil Temperature and Moisture (st1, sm1, st1, sm1)
BMX Sensor Readings (p1, t1, h1, p2, t2, h2)

{
 "name": "station-01 -> FS for chordsrt.com",
 "event": "FS",
 "deviceID": "Place Device ID Here",
 "url": "<http://3d.chordsrt.com/measurements/url_create>",
 "requestType": "GET",
 "noDefaults": true,
 "rejectUnauthorized": false,
 "query": {
     "key": "Place Chords Token ID Here",
     "instrument_id": "53",
     "at": "{{at}}",
     "hth": "{{hth}}",
     "bcs": "{{bcs}}",
     "bpc": "{{bpc}}",
     "cfr": "{{cfr}}",
     "css": "{{css}}",
     "bp1": "{{bp1}}",
     "bt1": "{{bt1}}",
     "bh1": "{{bh1}}",
     "hh1": "{{hh1}}",
     "ht1": "{{ht1}}",
     "st1": "{{st1}}",
     "sh1": "{{sh1}}",
     "st2": "{{st2}}",
     "sh2": "{{sh2}}",
     "hdt1": "{{hdt1}}",
     "hdh1": "{{hdh1}}",
     "hdt2": "{{hdt2}}",
     "hdh2": "{{hdh2}}",
     "sv1": "{{sv1}}",
     "si1": "{{si1}}",
     "su1": "{{su1}}",
     "vlx": "{{vlx}}",
     "blx": "{{blx}}",
     "mt1": "{{mt1}}",
     "rg": "{{rg}}",
     "rgt": "{{rg}}",
     "rgp": "{{rgp}}",
     "rg2": "{{rg2}}",
     "rgt2": "{{rgt2}}",
     "rgp2": "{{rgp2}}",
     "a4r": "{{a4r}}",
     "a5r": "{{a5r}}",
     "ws": "{{ws}}",
     "wd": "{{wd}}",
     "wg": "{{wg}}",
     "wgd": "{{wgd}}",
     "sg1": "{{sg1}}",
     "sg1v": "{{sg1v}}",
     "sg1p1": "{{sg1p1}}",
     "sg1t1": "{{sg1t1}}",
     "sg1h1": "{{sg1h1}}",
     "sg1p2": "{{sg1p2}}",
     "sg1t2": "{{sg1t2}}",
     "sg1h2": "{{sg1h2}}",
     "tlww": "{{tlww}}",
     "tlwt": "{{tlwt}}",
     "tsme25": "{{tsme25}}",
     "tsmeec": "{{tsmeec}}"
     "tsmvwc": "{{tsmvwc}}",
     "tsmt": "{{tsmt}}",
     "tmsms1": "{{tmsms1}}",
     "tmsms2": "{{tmsms2}}",
     "tmsms3": "{{tmsms3}}",
     "tmsms4": "{{tmsms4}}",
     "tmsmt1": "{{tmsmt1}}",
     "tmsmt2": "{{tmsmt2}}"
  }
}
</pre>
</div>
Example of Particle Webhook for Feather LoRaRemoteMO deployments.
If you define the below with no "deviceID". The webhook can be used by multiple devices.
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 200px; border: 1px solid black; padding: 10px;">
<pre>
{
 "name": "LR for chordsrt.com",
 "event": "LR",
 "deviceID": "Place Device ID Here",
 "url": "<http://3d.chordsrt.com/measurements/url_create>",
 "requestType": "GET",
 "noDefaults": true,
 "rejectUnauthorized": false,
 "query": {
     "key": "Place Chords Token ID Here",
     "instrument_id": "{{id}}",
     "at": "{{at}}",
     "rg": "{{rg}}",
     "sg": "{{sg}}",

     "st1": "{{st1}}",
     "sm1": "{{sm1}}",
     "st2": "{{st2}}",
     "sm2": "{{sm2}}",

     "bp1": "{{bp1}}",
     "bt1": "{{bt1}}",
     "bh1": "{{bh1}}",
     "bp2": "{{bp2}}",
     "bt2": "{{bt2}}",
     "bh2": "{{bh2}}",

     "st1": "{{st1}}",
     "sh1": "{{sh1}}",
     "st2": "{{st2}}",
     "sh2": "{{sh2}}",

     "mt1": "{{mt1}}",
     "mt2": "{{mt2}}",

     "bv": "{{bv}}",
     "hth": "{{hth}}"
  }
}
</pre>
</div>


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

784### Time Management
A valid time source is required for normal operation and for observations to be made. There are 3 time clocks involved.
Network Time
When a network connection is made, time is obtained from the network and the System Clock is updated. A request can be made in software to perform this action. This is done every 4 hours.
#### Realtime Clock (RTC)
If the RTC is found at initialization and has a year greater equal to 2023 and less than or equal to 2031. We assume time on the RTC is valid. And we set the System Clock with this time.
If the RTC is not found or has invalid time. We must wait and get time from the network before we can make any observations.  
#### System Clock (STC)
This clock is maintained by the microcontroller. At start-up it has no valid time and must be set from the RTC or Network time. The System clock is updated every 4 hours from Network time.

Clock handling output messages during boot initialization
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 100px; border: 1px solid black; padding: 10px;">
<pre>
2000-01-01T00:00:15+ - System Time
2023-10-26T18:07:24*   - RTC Time
RTC:VALID                     - RTC Year >= 2023 and RTC Year <= 2031
STC: Valid  - STC set from RTC and we have a valid clock source.
2023-10-26T18:07:24= - System Time
</pre>
</div>

In the main code loop when System time is valid a check. The RTC clock is serviced. If the RTC invalid time. It is set from the System time clock. Which was set from network time.  Every 2 hours we update the RTC from System time.

In the main code loop when System time is valid and we are network connected, then every 4 hours we request network time. Which results in the System clock being updated.

### Daily Reboot
A loop counter is maintained and set so around every 22 hours the system reboots itself. This is done to try and bring back online any missing sensors.  If a WatchDog board is not connected a System reset is performed.

Particle Boron Power Down when no USB power and LiPo at 10%
Random board shut offs could cause SD corruption when in the middle of writing to the SD card. To prevent this, when the Boron board has no USB power and the LiPo battery is at or below 10 percent, the board is powered off.  The board will start back up again when USB power returns.


INFO is sent at boot. When we are going to power down do to low power,  And when we request it from particle console.  So the daily reboot will cause a INFO to be sent.


Pins Assignments as of release FSAC-240223
See the releases code and Fritzing Wiring Drawings current information
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 300px; border: 1px solid black; padding: 10px;">
<pre>

 D8   = Serial Console (Ground Pin to Enable) - Not on Grove Shield
 D7   = On Board LED - Lit when rain gauge tips, blinks when console connection needed
 D6   = Reserved for Lora IRQ - Not on Grove Shield
 D5   = SD Card Chip Select
 D4   = SPI1 MSIO - Reserved for LoRa
 D3   = SPI1 MOSI - Reserved for LoRa
 D2   = SPI1 SCK  - Reserved for LoRa
 D1   = I2C SCL
 D0   = I2C SDA

 A0   = WatchDog Monitor/Relay Reset Trigger
 A1   = WatchDog Monitor Heartbeat
 A2   = Wind Speed IRQ
 A3   = Rain Gauge IRQ
 A4   = 2nd Rain Gauge, Distance Gauge based on SD card file existing, or Raw pin value
 A5   = Future
 D13  = SPIO CLK   SD Card
 D12  = SPI0 MOSI  SD Card
 D11  = SPI0 MISO  SD Card
 D10  = UART1 RX - Reserved for LoRa CS
 D9   = UART1 TX - Reserved for LoRa RESET
</pre>
</div>

### PM25AQI - I2C - Air Quality Sensor

Data variables returned
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 350px; border: 1px solid black; padding: 10px;">
<pre>

  pm10_standard  < Standard Particle PM1.0 concentration unit µg 𝑚3
  pm25_standard  < Standard Particle PM2.5 concentration unit µg 𝑚3
  pm100_standard < Standard Particle PM10.0 concentration unit µg 𝑚3
  pm10_env        < Atmospheric Environmental PM1.0 concentration unit µg 𝑚3
  pm25_env        < Atmospheric Environmental PM2.5 concentration unit µg 𝑚3
  pm100_env      < Atmospheric Environmental PM10.0 concentration unit µg 𝑚3
  particles_03um < Particles with diameter beyond 0.3 µ 𝑚 in 0.1L of air
  particles_05um < Particles with diameter beyond 0.5 µ 𝑚 in 0.1L of air
  particles_10um < Particles with diameter beyond 1.0 µ 𝑚 in 0.1L of air
  particles_25um < Particles with diameter beyond 2.5 µ 𝑚 in 0.1L of air
  particles_50um < Particles with diameter beyond 5.0 µ 𝑚 in 0.1L of air
  particles_100um   < Particles with diameter beyond 10.0 µ 𝑚 in 0.1L of air

  pms = Particulate Matter Standard
  pme = Particulate Matter Environmental

  Variable Tags for what we monitor and report on
  pm1s10
  pm1s25
  pm1s100
  pm1e10
  pm1e25
  pm1e100
</pre>
</div>

### MaxBotix Distance Sensors

#### The 5-meter sensors (MB7360, MB7369, MB7380, and MB7389) use a scale factor of (Vcc/5120) per 1-mm.
<pre>
Particle 12bit resolution (0-4095)
Sensor has a resolution of 0 - 5119mm
Each unit of the 0-4095 resolution is 1.25mm

Feather has 10bit resolution (0-1023)
Sensor has a resolution of 0 - 5119mm
Each unit of the 0-1023 resolution is 5mm
</pre>
#### The 10-meter sensors (MB7363, MB7366, MB7383, and MB7386) use a scale factor of (Vcc/10240) per 1-mm.
<pre>
Particle 12bit resolution (0-4095)
Sensor has a resolution of 0 - 10239mm
Each unit of the 0-4095 resolution is 2.5mm

Feather has 10bit resolution (0-1023)
Sensor has a resolution of 0 - 10239mm
Each unit of the 0-1023 resolution is 10mm
</pre>

### Station Information (Event type "INFO")
At boot the station will send a event message of type "INFO" to Particle. This message contains configuration and status information. You can also request an INFO event message to be sent from the device via DoAction Function on the Particle Console. Use keyword "INFO". Every INFO execution will create/rewrite file INFO.TXT with INFO information.

INFO event message from a Boron
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 500px; border: 1px solid black; padding: 10px;">
<pre>
{
"devid": "e00fce68bde8f63890a3b118"
"devos":"4.1.0"
"freemem":57736
"uptime":80
"board": "boron"
"at": "2024-10-30T21:09:37"
"ver": "FSAC-241029v33
"hth": 4009985
"obsi": "60s"
"obsti": "15m"
"t2nt": "783s"
"drct": "4624"
"n2s": "NF"
"ps": "VIN"
"bcs": "CHARGED"
"bpc": 100
"css": 64.9989
"csq": 37.499
"imsi": "234103519249568"
"actsim": "INTERNAL"
"a4": "DIST 5M"
"sensors": "BMX1(BMP390),MCP1,SHT1,HIH8,SI,AS5600,HI,WBT,WBGT WO/GLOBE"
"oled": "32"
"scepin": "DISABLED"
"sce": "TRUE"
}

ps = power source
bv = battery voltage
bcs = battery charging status
obsi = observation interval (seconds)
obsti = observation transmit interval (minutes)
t2nt = time to next transmit (seconds)
drct = daily reboot countdown timer (counter)
sce = serial console enabled
scepin = status of the actual serial console pin
a4 = configuration of this pin (DIST 5M, DIST 10M, RG2, NS[Not Set])
</pre>
</div>

INFO event message from a Argon
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 480px; border: 1px solid black; padding: 10px;">
<pre>
 {
"devid": "e00fce68a396d11b8cafc55f"
"devos":"4.1.0"
"freemem":57736
"uptime":80
"type": "argon"
"at": "2024-10-30T20:43:42"
"ver": "FSAC-241029v33"
"hth": 0
"obsi": "60s"
"obsti": "15m"
"t2nt": "236s"
"drct": "4624"
"n2s": "NF"
"ps": "USB"
"bv": 4.16972
"bcs": "!CHARGING"
"wss": 100
"wsq": 100
"mac": "e8:9f:6d:be:6d:28"
"ip": "10.0.0.1"
"mask": "255.255.255.0"
"gateway": "10.0.0.254"
"dns": "10.0.0.253"
"dhcps": "10.0.0.253"
"ssid": "SSID String"
"bssid": "cc:40:d0:87:3a:aa"
"a4": "DIST 5M"
"sensors": "BMX1(BMP390),MCP1,SHT1,HIH8,SI,AS5600,HI,WBT,WBGT WO/GLOBE"
"oled": "32"
"scepin": "DISABLED"
"sce": "TRUE"
}

wss = wireless signal strength (percent)
wsq = wireless signal  quality (percent)
bssid = mac address of the wireless gateway
</pre>
</div>