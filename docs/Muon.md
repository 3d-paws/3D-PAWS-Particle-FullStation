# Muon Board Support
[←](../README.md)<BR>

Particle Muon Datasheet https://docs.particle.io/reference/datasheets/m-series/muon-datasheet

Requires a SD Card to be connected to GPIO pins

### WIFI Support
To Enable Muon WiFi

At the top level of the SD card make a file called WIFI.TXT
Add one line to the file
This line has 3 items that are comma separated Example
 
 MUON,ssid,password
 
 MUON is a keyword to distinguish the file from a Argon WIFI.TXT file.

 If WIFI.TXT is removed, cellular is enabled.

### Muon Compiling
In library.properties uncomment
#dependencies.particle-som-gnss=1.0.0

Copy lib-muonparticle-som-gnss to lib directory


### Caveats
* Muon can not support Tinovi Leaf Wetness sensor. I2C 0x61 conflicts with KG200Z LoRaWAN radio. Removed from Muon * compile.
* Muon can not support HTU21DF sensor. I2C 0x40 conflicts with the on board fuel gauge chip. Removed from Muon compile.
* Muon wind direction requires a AS5600L sensor. The AS5600 version has same I2C address (0x36) as the on board cipher chip.
* Muon has a on board temperature sensor that will be reported as ptms (Particle Muon Temperature Sensor)

### Muon Pins
```
Pin  Name
LORA MODULE
8  D9  MOSI SPI1
10 D10 MISO SPI1
36 D3  LORA_SS
38 D21 LORA_RESET
40 D20 LORA_IRQ_PIN - G0 on LoRa board

SERIAL CONSOLE 
26  D17/A2 SCE_PIN

SD CARD
17  3v3
19  SPI MOSI
21  SPI MISO
23  SPI SCK
25  GND
24  D29/A6  CS

LED
22  D22 LED_PIN - Optional if you want an on board LED. 

GPS
16 D24 Serial2 TX (UART transmit from MCU)
18 D25 Serial2 RX (UART receive to MCU)

GROVE PWM
15  D27 RELAY/REBOOT PIN
14  DO NOT USE

GROVE PWM
32 D5 Wind
33 D4 Rain1

GROVE D5 (Optional Configuration Pins)
29 A0/D19 OP1 PIN (Rain2, Distance, Raw)
31 A1/D18 OP2 PIN (Raw)
```

### Muon on board chip I2C addresses
```
0x28: STUSB4500 USB-C power controller
0x36: MAX17043 Fuel Gauge
0x48: TMP112A temperature sensor
0x61: KG200Z LoRaWAN radio
0x69: AM1805 RTC/Watchdog
0x6B: bq24195 Power Management IC (PMIC)
```

