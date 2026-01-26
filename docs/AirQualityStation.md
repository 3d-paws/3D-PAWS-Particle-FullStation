# Air Quality Station Option and Operation
[←Top](../README.md)<BR>

### Air Quality Station
You can change the operating behavior of the Full Station to become a dedicated Air Quality Station (AQS). Use the Plantower PMSA003I sensor. 
When set as a AQS:
- Wind, Rain and other OP[1,2] sensors are disabled. Files OP1DIST.TXT, OP1RAIN.TXT, OP1RAW.TXT, OP2RAW.TXT are ignored.
- One minute observations becomes 5 minutes (default), 10 or 15. With matching reporting periods.
- OP2 Pin is wired to SET pin on the air quality sensor. The SET pin on the air quality sensor, when low, puts the sensor into sleep mode. \
  Sleep mode on the air quality sensor is a feature that allows the sensor to enter a low-power state to conserve energy and extend the lifetime of the laser and fan components. \
  This reduces wear on moving parts, especially the fan and laser. Both of which have a finite lifespan (laser is about 8000 hours).
- In AQS mode the station publishes to Particle as event type "FS". Like the Full station. No change.

#### Taking Air Quality Observations
- The air quality sensor is turn on (via OP2 Pin) 45 seconds prior to the time we need to make the observation.
- After 35 seconds (required for warm-up and stabilization) a reading is taken. This is discarded (clears any stale or buffered data).
- Then 800ms later, 11 readings are taken 800ms a part.
- Then sensor is then put to sleep.
- From the 11 readings, if there are more failed readings than good, -999 is reported for the sensor readings. \
  Otherwise an average is returned from the good samples taken.

#### Enabling/Disabling Air Quality Station Option
- Create/Delete file OPTAQS.TXT on the SD card.
- You can also use the Particle Console to send Do Actions.\
  OPTAQS - Air Quality Station configuration (OPTAQS.TXT) (Requires REBOOT after setting) \
  OPTFS - Full Station configuration (Removes OPTAQS.TXT) (Requires REBOOT after setting)

#### Caveats
- Other I2C sensors from the Full Station are supported.
- The air quality sensor should be connected to the 5v pin and not the 3.3v. \
  Not doing so put a strain on the on board 3.3v voltage regulator.

### Serial Console Output Example
  <div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 200px; border: 1px solid black; padding: 10px;">
<pre>
OLED:Disabled
SC:Enabled
SER:OK
Copyright [2025] [University Corporation for Atmospheric Research]
FS-250908v40
RR:Reset Pin (button or pin) (20,0)
SD:Online
SD:OBS DIR Exists
N2S:None
A4A5FU:START
SDRN:A4DIST.TXT NF
SDRN:A4RAIN.TXT NF
SDRN:A4RAW.TXT NF
SDRN:A5RAW.TXT NF
SDRN:5MDIST.TXT NF
A4A5FU:END
CF:aes_pkey=[...]
CF:aes_myiv=[...]
CF:lora_unitid=[1]
CF:lora_txpower=[5]
CF:lora_freq=[915]
EEPROM DUMP
 LEN:4096
 RT1:0.00
 RP1:0.00
 RT2:0.00
 RP2:0.00
 RGTS:1757356921
 N2SFP:0
 CS:1757356921
 CSC:1757356921
2000-01-01T00:00:10S
2025-09-08T18:42:16*
RTC:VALID
2025-09-08T18:42:16S
STC:VALID
2025-09-08T18:42:16S
PS:2
BS:6
BPC:-1.00
SIM:Internal
SIM:NO UPDATE FILE
OBSAQS:INIT         <------------- Notice!!
OPTAQI Enabled      <------------- Notice!!
OBSTXI:INIT
OBI Corrected 5M
OBI=5M, TXI=15M
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
BMX:INIT
get_Bosch_ChipID()
  I2C:77 Reg:00
  ERR_ET:7
  I2C:77 Reg:D0
  ERR_ET:7
BMX_1 NF
get_Bosch_ChipID()
  I2C:76 Reg:00
  ERR_ET:7
  I2C:76 Reg:D0
  ERR_ET:7
BMX_2 NF
HTU21D:INIT
HTU NF
MCP9808:INIT
MCP1 NF
MCP2 NF
MCP3 NF
MCP4 NF
SHT:INIT
SHT1 NF
SHT2 NF
HIH8:INIT
HIH8 NF
SI1145:INIT
SI:NF
VLX:INIT
VLX NF
BLX:INIT
BLX:NF
AS5600:INIT
WD:NF
PM25AQI:INIT
PM:OK
HDC:INIT
HDC1 NF
HDC2 NF
LPS:INIT
LPS1 NF
LPS2 NF
TLW:INIT
TLW NF
TMSM:INIT
TMSM NF
WBT:INIT
WBT:NF
HI:INIT
HI:NF
WBGT:INIT
WBGT:NF
AES_KEY[...]
AES_MYIV[...]
LORA CFV OK
LORA NF
DoAction:OK
IMSI:NF
T>6, RT>6 - OK
AQS:WAKEUP
...................................
AQS:Take Reading
AQS:SLEEP
AQS:OK
MUX:OBSDO
OBS[0]->SD
{"at":"2025-09-08T18:43:00","css":-1.0000,"hth":33029705,"bcs":6,"bpc":0.0,"cfr":0,"pm1s10":3,"pm1s25":3,"pm1s100":3,"pm1e10":3,"pm1e25":3,"pm1e100":3,"tsme25-1":1.0,"tsmec-1":10.2,"tsmvwc-1":0.0,"tsmt-1":23.8}
Connected
{"at":"2025-09-08T18:43:00","css":34.9996,"hth":33029705,"bcs":6,"bpc":0.0,"cfr":0,"pm1s10":3,"pm1s25":3,"pm1s100":3,"pm1e10":3,"pm1e25":3,"pm1e100":3,"tsme25-1":1.0,"tsmec-1":10.2,"tsmvwc-1":0.0,"tsmt-1":23.8}
FS[0]->PUB OK[211]
2025-09-08T18:43:02
CS:37.49 B:6,0.00
RTC:1ST SYNC
2025-09-08T18:43:03R
INFO_DO()
INFO->SD OK
{"devid":"e00fce68bde0000090a3b118","devos":"6.3.3","freemem":19400,"uptime":58,"type":"boron","mode":"AQS","at":"2025-09-08T18:43:03","ver":"FS-250908v40","hth":64,"rr":"20-0","obsi":"5m","obsti":"15m","t2nt":"898s","drct":79199,"n2s":"NF","ps":"USB_HOST","bcs":"MISSING","bpc":-1.0,"css":37.4990,"csq":29.1661,"imsi":"","actsim":"INTERNAL","sensors":"TSM1(7.0),PM25AQ","lora":"NF","oled":"NF","scepin(D8)":"DISABLED","sce":"TRUE"}
INFO->PUB OK[433]
</pre>