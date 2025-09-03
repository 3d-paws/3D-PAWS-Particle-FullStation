# Serial Monitor
[←Top](../README.md)<BR>
Adding a jumper wire between Particle pin D8 (Boron & Argon), A2 (Muon) and ground will enable serial text output on the USB port at boot time.

A serial monitor from Arduino's IDE can be used. TIP: With the Arduino serial monitor. Select "Both NL & CR" on the pull down menu at the bottom.  On a Mac with Visual Studio installed with Particle's Development Environment; the shell command "particle serial monitor" can be used.

Upon Particle board boot with the jumper wire connected, software will wait 60 seconds for you to connect the serial monitor. Flashing the board led.  After 60 seconds the software will continue the boot process. Below is an example of what you might see as the software initializes and discovers connected devices.

<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 200px; border: 1px solid black; padding: 10px;">
<pre>
$ particle serial monitor
Opening serial monitor for com port: "/dev/tty.usbmodem14101"
Serial monitor opened successfully

OLED:Enabled
SC:Enabled
SER:OK
Copyright [2024] [University Corporation for Atmospheric Research]
FSAC-241206v37
SD:Online
SD:OBS DIR Exists
N2S:None
CF:aes_pkey=[....]
CF:aes_myiv=[....]
CF:lora_unitid=[1]
CF:lora_txpower=[23]
CF:lora_freq=[915]
EEPROM DUMP
 LEN:4096
 RT1:0.20
 RP1:0.00
 RT2:0.00
 RP2:0.00
 RGTS:1733949583
 N2SFP:0
 CS:1733949583
 CSC:1733949583
2000-01-01T00:00:16+
2024-12-11T20:41:15*
RTC:VALID
STC: Valid
2024-12-11T20:41:15=
SIM:Internal
SIM:NO UPDATE FILE
TXI:INIT
TXI5M Found
TXI=5M
A4:INIT
A4=DIST
DIST=5M
A5:INIT
A5=NF
BMX:INIT
get_Bosch_ChipID()
  I2C:77 Reg:00
  CHIPID:60 BME/390
BMP390_1 OK
get_Bosch_ChipID()
  I2C:76 Reg:00
  ERR_ET:7
  I2C:76 Reg:D0
  ERR_ET:7
BMX_2 NF
HTU21D:INIT
HTU NF
MCP9808:INIT
MCP1 OK
MCP2 NF
MCP3 NF
MCP4 NF
SHT:INIT
SHT1 OK
SHT2 NF
HIH8:INIT
HIH8 OK
SI1145:INIT
SI:NF
VLX:INIT
VLX OK
BLX:INIT
BLX:OK
AS5600:INIT
WD:OK
PM25AQI:INIT
PM:NF
HDC:INIT
HDC1 OK
HDC2 OK
LPS:INIT
LPS1 OK
LPS2 NF
TLW:INIT
TLW NF
TSM:INIT
TSM NF
TMSM:INIT
TMSM NF
WBT:INIT
WBT:OK
HI:INIT
HI:OK
WBGT:INIT
WBGT:OK wo/Globe
AES_KEY[....]
AES_MYIV[....]
LORA CFV OK
LORA NF
DoAction:OK
WindDist Init()
............................................................
IMSI:234103519249568
T>6, RT>6 - OK
EEPROM RT UPDATED
OBS[0]->SD
{"at":"2024-12-11T20:42:23","css":42.4994,"hth":17397249,"bcs":3,"bpc":96.9,"cfr":0,"rg":0.0,"rgt":0.2,"rgp":0.0,"ws":0.0,"wd":181,"wg":0.0,"wgd":-999,"bp1":843.6,"bt1":22.5,"st1":22.0,"sh1":30.5,"hdt1":22.2,"hdh1":29.3,"hdt2":22.2,"hdh2":29.3,"lpt1":17.0,"lpp1":843.1,"ht2":23.3,"hh2":29.3,"mt1":22.2,"vlx":7.0,"blx":293.5,"sg":431.0,"hi":21.8,"wbt":12.6,"wbgt":17.1}
DB:OBS_Exit
RTC: 1ST SYNC
2024-12-11T20:42:26*
INFO_DO()
{"devid":"e00fce68bde8f63590a3b118","devos":"6.1.1","freemem":56248,"uptime":88,"board":"boron","at":"2024-12-11T20:42:26","ver":"FSAC-241206v37","hth":17397249,"obsi":"60s","obsti":"5m","t2nt":"212s","drct":79200,"n2s":"NF","ps":"VIN","bcs":"CHARGED","bpc":96.9,"css":42.4994,"csq":37.4990,"imsi":"234103519249568","actsim":"INTERNAL","a4":"DIST 5M","sensors":"BMX1(BMP390),MCP1,SHT1,HDC1,HDC2,LPS1,HIH8,VEML,BLX,AS5600,HI,WBT,WBGT WO/GLOBE","oled":"32","scepin":"ENABLED","sce":"TRUE"}
INFO->PUB OK[488]
Connected
{"at":"2024-12-11T20:42:23","css":54.9996,"hth":17397249,"bcs":3,"bpc":96.9,"cfr":0,"rg":0.0,"rgt":0.2,"rgp":0.0,"ws":0.0,"wd":181,"wg":0.0,"wgd":-999,"bp1":843.6,"bt1":22.5,"st1":22.0,"sh1":30.5,"hdt1":22.2,"hdh1":29.3,"hdt2":22.2,"hdh2":29.3,"lpt1":17.0,"lpp1":843.1,"ht2":23.3,"hh2":29.3,"mt1":22.2,"vlx":7.0,"blx":293.5,"sg":431.0,"hi":21.8,"wbt":12.6,"wbgt":17.1}
FS[0]->PUB OK[369]
2024-12-11T20:42:30
CS:52.49 B:3,96.91
</pre>
</div>
