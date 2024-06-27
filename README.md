# 3D-PAWS Particle Full Station

Last Updated: 20240627

## Description

This software is supported on a Particle Boron and Argon boards. It provides full weather station features.

## Features

### Serial Monitor

Adding a jumper wire between Particle pin D8 and ground will enable serial text output on the USB connector at boot time. Software is configured to run at 9600 baud.  

A serial monitor from Arduino's IDE can be used. On a Mac with Visual Studio installed with Particle's Development Environment; the shell command "particle serial monitor" can be used.

Upon Particle board boot with the jumper wire connected, software will wait 60 seconds for you to connect the serial monitor. Flashing the board led.  After 60 seconds the software will continue the boot process. Below is an example of what you might see as the software initializes and discovers connected devices.

<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 200px; border: 1px solid black; padding: 10px;">
<pre>
$ particle serial monitor
Opening serial monitor for com port: "/dev/tty.usbmodem14101"
Serial monitor opened successfully:

OLED:Enabled
SC:Enabled
SER:OK
FSAC-231025
SD:Online
SD:OBS DIR Exists
N2S:None
EEPROM DUMP
 LEN:4096
 RT:0.20
 RP:0.00
 R2T:0.00
 R2P:0.00
 RGTS:1698340415
 N2SFP:0
 CS:1698340415
 CSC:1698340415
2000-01-01T00:00:15+
2023-10-26T18:07:24*
RTC:VALID
STC: Valid
2023-10-26T18:07:24=
SIM:Internal
SIM:NO UPDATE FILE
A4:INIT
A4=DIST
DIST=10M
BMX:INIT
get_Bosch_ChipID()
  I2C:77 Reg:00
  ERR_ET:3
  I2C:77 Reg:D0
  ERR_ET:3
BMX_1 NF
get_Bosch_ChipID()
  I2C:76 Reg:00
  ERR_ET:3
  I2C:76 Reg:D0
  ERR_ET:3
BMX_2 NF
HTU21D:INIT
HTU OK
MCP9808:INIT
MCP1 OK
MCP2 NF
SHT:INIT
SHT1 NF
SHT2 NF
HIH8:INIT
HIH8 NF
SI1145:INIT
SI:OK
SI:VI[264.00]
SI:IR[268.00]
SI:UV[0.03]
LUX:INIT
LUX NF
AS5600:INIT
WD:NF
PM25AQI:INIT
PM:OK
LORA NF
DoAction:OK
WindDist Init()
............................................................
OBS[0]->SD
{"at":"2023-10-26T18:08:31","css":64.9989,"hth":258113,"bcs":3,"bpc":94.6,"cfr":0,"rg":0.0,"rgs":66,"rgt":0.2,"rgp":0.0,"ws":0.0,"wd":-999,"wg":0.0,"wgd":-999,"hh1":24.4,"ht1":22.1,"sv1":261.0,"si1":267.0,"su1":0.0,"mt1":22.1,"sg":974.0,"pm1s10":19,"pm1s25":27,"pm1s100":34,"pm1e10":19,"pm1e25":27,"pm1e100":34}
R:00,S:00,D:NF  3F041
BMX:NF

H:24.40T22.12 C64.99
V262,267,0 B3,94.61
</pre>
</div>

### OLED Display

If an OLED display is detected at startup, messages are also sent to this display. Jumper wire is not needed for activation.

Station Monitor
Upon setup completion.  If the jumper wire is in place then the Station Monitor will run instead of the main work loop.  The station monitor will display various sensor values. Updating every second. A good way to check rain gauge tips and wind direction.

R:00,S:00,D:NF  3F041
BMX:NF
H:24.40T22.12 C64.99
V262,267,0 B3,94.61

The station monitor will run for 10 minutes. After which, the main work loop will be started. This is to safeguard against the jump wire being left connected on a deployment in the field.

Caveat: If you reapply the jumper after removing, the station monitor will restart and run for the remaining time left of the 10 minutes allotted.

### 3rd Party SIM Support

On the underside of the Particle Boron board is a slot for a SIM card. This SIM slot can be used instead of the built-in SIM. Particle's software stores the state of what SIM is being used and cell provider information in nvram. The state is either set INTERNAL or EXTERNAL. We can control this state by the existence of the SD file SIM.TXT.  The user needs to create file SIM.TXT and place it at the top directory on the SD card.

If the SIM.TXT file contains the below and it passes syntax checks. Then at boot software sets the SIM to use to EXTERNAL and the file is then renamed to SIMOLD.TXT, so we don't do this on the next boot. Below shows options for the SIM.TXT. What is used will depend on what is needed by the SIM. Select one of the 3 options below.

Option 1: Connect to a cellular network using APN, USERNAME and PASSWORD

>AUP epc.tmobile.com username passwd

Option 2: Connect to a cellular network using USERNAME and PASSWORD

>UP username password

Option 3: Connect to a cellular network using only APN

>APN apn

Below shows an example of what was needed for the T-Mobile SIM.

>APN epc.tmobile.com

If the SIM.TXT file contains the below and it passes syntax checks. Then at boot software sets the SIM to use to INTERNAL and the file is then renamed to SIMOLD.TXT, so we don't do this on the next boot.

>INTERNAL

If you have the serial console enabled, you will see messages related to the above actions. Upon SIM setup completion, the board led will flash forever. Indicating a reboot is needed.

SEE: <https://docs.particle.io/troubleshooting/guides/connectivity-troubleshooting/using-3rd-party-sim-cards/>

### Argon WiFi Board Support

When setting up an Argon for the first time you use the Particle phone app and pair with the Argon board. This process includes configuring the WiFi settings.  These settings are saved in nonvolatile memory.  At this point you are done. But, if you want to change WiFi settings stored in nonvolatile memory you can: Use the app on your phone or create a file on the SD card with the WiFi credentials.

WiFi credentials file  
At the top level of the SD card, create a file called WIFI.TXT. This file has one line with 3 comma separated items. Example:

>AuthType,ssid,password

Where AuthType is one of these keywords  
>UNSEC  
WEP  
WPA  
WPA2  
WPA_ENTERPRISE  
WPA2_ENTERPRISE  

When WiFi Information is successfully read and parsed from the file at startup, the below will display on Serial Console:
WIFI:Open
WIFI:ATYPE[WPA2]
WIFI:SSID[NotMySSID]
WIFI:PW[NotMyPassword]
WIFI:Credentials Cleared
WIFI:Credentials Set [One of the above AuthTypes]

If WiFi file not found the below will display on Serial Console:  
>WIFI:NOFILE USING NVAUTH

Note: If something went wrong obtaining WiFi credentials the credentials in non volatile memory are used.

### DoAction Functions on Particle Cloud Console

On the view device screen on the Particle Console there is a FUNCTIONS area at the lower right. This is used to send commands to the online device.

Reboot device. Toggle pin A0. If no relay/watchdog is connected to A0, then a soft boot on the Particle board is performed.
>REBOOT

Clear rain totals that are stored in nvram.

>CRT
Configures pin A4 to be a 2nd rain gauge. This function creates file A4RAIN.TXT on the SD card. A reboot is necessary after running this command.

>A4RAIN

Configures pin A4 to be a distance gauge. This function creates file A4DIST.TXT on the SD card. A reboot is necessary after running this command. By default the distance sensor is 10m. Values read from pin A4 are divided by 4.

>A4DIST

Creates file 5MDIST.TXT and values read from pin A4 are divided by 8.
>5MDIST

Unassigns pin A4. Deletes files A4RAIN.TXT, A4DIST.TXT, 5MDIST.TXT if they exist
>A4CLR

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
  /A4RAIN.TXT  Set pin A4 to be a 2nd rain gauge.  
  /A4DIST.TXT  Set pin A4 to be a distance gauge of type 10m  
  /5MDIST.TXT  Set distance gauge of type 5m  
  /SIM.TXT  Support 3rd party SIM.  
  /WIFI.TXT  Support for Argon WiFi Boards. Stores WiFi information  
</pre>
</div>

## Code Operation

### Timing
Every minute software takes an observation and stores it to the SD card in a dated file. Which is located under the /OBS folder.

Every second, Wind Speed & Direction occurs. If sensors for Air Particle and Distance are attached, these are also sampled.

Within the 1 second window Particle pin A1 is toggled on for 250ms. Providing the board's heartbeat.  If LoRa is activated, inbound messages are for, checked multiple times.

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
One minute observations are sent to Particle Cloud with Particle Event Name "FN". This is used by the Particle Cloud service to identify and associate webhooks for triggering.

After transmitting an observation, software will wait for a confirmation result code from Particle Cloud. If the result code reports failure or we didn't have network connectivity at the time of transmission. The observation is appended to the Need to Send file located on the SD card at the top level and called N2SOBS.TXT. If the file does not exist, it is created then appended to. Observations in this file will later be transmitted.

Caveat: At the time of appending the observation to the N2S file, if the file is greater than 512 *60* 48 bytes. ~2 days of observations. The file is deleted. Recreated and the observation is appended.

### Sending N2S Observations
After successful completion of current 1 minute observations have finished transmitting. If N2S observations exist, these are then read from the file and transmitted.  Position in the N2S file of what has been transmitted is maintained in nvram. So rebooting does not cause the retransmission of N2S observations. On a N2S observation transmit failure, N2S processing stops until the next 15 minute window.  While sending N2S observations, 1 second tasks and 1 minute observations still occur. When storage space for 1 minute observations becomes full. N2S processing stops and the main work loop takes over.  At which point current 1 minute observations will be transmitted. Freeing up the observation storage.

### LoRa Observations
When LoRa is enabled, the full station will receive LoRa messages from remote stations, store the observations and transmit them at the 15 minute window.

### LoRa Stream Gauge
When a LoRa stream gauge message is received, the message is saved and later added as to the next 1 minute observation. Additional messages received prior to the 1 minute observation overwrite the prior.

 Stream Gauge (sg1)
 BMX Sensor Readings (sg1p1, sg1t1, sg1h1, sg1p2, sg1t2, sg1h2)
 Battery Voltage (sg1v)

### Rain and Soil Moisture

After the Full Station 1 minute observations are sent at the 15 minute window. LoRa rain and soil moisture observations (RS) which were received and stored since the last 15 minute transmit will be sent.

RS observations are sent to Particle Cloud with Particle Event Name "RS".

The Full Station can support receiving from 10 different LoRa RS devices. The RS devices are configured to transmit a unique ID which maps to a Chords ID.  The Full Station will transmit a separate message to Particle's Cloud service for each RS device. If the Full Station receives multiple messages from a RS device in the 15 minute window, rain gauge data is  summed. Rest of the observation data is overwritten with the newer observation received.

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
     "sv1": "{{sv1}}",
     "si1": "{{si1}}",
     "su1": "{{su1}}",
     "mt1": "{{mt1}}",
     "rg": "{{rg}}",
     "rgs": "{{rgs}}",
     "rgt": "{{rg}}",
     "rgp": "{{rgs}}",
     "rg2": "{{rg2}}",
     "rg2s": "{{rg2s}}",
     "rg2t": "{{rg2}}",
     "rg2p": "{{rg2s}}",
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
     "sg1h2": "{{sg1h2}}"
 }
}
</pre>
</div>
Example of Particle Webhook for LoRa Rain and Soil (RS)
With no "deviceID" defined in the below webhook. It can be used by multiple devices.  
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 200px; border: 1px solid black; padding: 10px;">
<pre>
{
 "name": "RS for chordsrt.com",
 "event": "RS",
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
     "st1": "{{st1}}",
     "sm1": "{{sm1}}",
     "st2": "{{st2}}",
     "sm2": "{{sm2}}",
     "p1": "{{p1}}",
     "t1": "{{t1}}",
     "h1": "{{h1}}",
     "p2": "{{p2}}",
     "t2": "{{t2}}",
     "h2": "{{h2}}",
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
 AREA        BIT       COMMENT
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
LORA         0x2000    Set if LoRa Radio missing at startup
SHT_1        0x4000    Set if SHTX1 Sensor missing
SHT_2        0x8000    Set if SHTX2 Sensor missing
HIH8         0x10000   Set if HIH8000 Sensor missing
LUX          0x20000   Set if VEML7700 Sensor missing
PM25AQI      0x40000   Set if PM25AQI Sensor missing
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

### Time Management
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

Pins Assignments as of release FSAC-240223
See the releases code and Fritzing Wiring Drawings current information
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 300px; border: 1px solid black; padding: 10px;">
<pre>

 D8   = Serial Console (Ground Pin to Enable) - Not on Grove Shield
 D7   = On Board LED - Lit when rain gauge tips, blinks when console connection needed
 D6   = Not in use - Not on Grove Shield
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
 A4   = 2nd Rain Gauge or Distance Gauge based on SD card file existing
 A5   = Reserved for Lora IRQ
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