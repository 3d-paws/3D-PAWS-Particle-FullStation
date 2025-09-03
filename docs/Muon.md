# Muon Board Support
[← Back to Top](../README.md)

---

**Particle Muon Datasheet:**  
[Particle Muon Datasheet (official)](https://docs.particle.io/reference/datasheets/m-series/muon-datasheet)

---

## SD Card Requirement
A microSD card must be connected to the GPIO pins for certain functionality.

---

## Wi-Fi Support

To enable Muon Wi-Fi:  

At the **top level of the SD card**, create a file named `WIFI.TXT`.  
Add one line with three comma-separated items:  

MUON,ssid,password

- The keyword `MUON` distinguishes this from an Argon `WIFI.TXT` file.  
- If the `WIFI.TXT` file is removed, cellular connectivity is enabled instead.

---

## Muon Compiling

- In `library.properties`, uncomment the line:  
  #dependencies.particle-som-gnss=1.0.0  
- Copy the `lib-muonparticle-som-gnss` folder to the `lib` directory.

---

## Caveats

- Muon cannot support the Tinovi Leaf Wetness sensor due to I2C address (0x61) conflict with the KG200Z LoRaWAN radio. The sensor is removed from the Muon compile.  
- Muon cannot support the HTU21DF sensor due to I2C address (0x40) conflict with the onboard fuel gauge chip. It is removed from Muon compile.  
- Muon wind direction requires an AS5600L sensor. The AS5600 version shares the same I2C address (0x36) as the onboard cipher chip.  
- Muon includes an onboard temperature sensor reported as ptms (Particle Muon Temperature Sensor).

---

## Muon Pins

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
22  D22 LED_PIN - Optional onboard LED  

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

---

## Muon Onboard Chip I2C Addresses

0x28: STUSB4500 USB-C power controller  
0x36: MAX17043 Fuel Gauge  
0x48: TMP112A temperature sensor  
0x61: KG200Z LoRaWAN radio  
0x69: AM1805 RTC/Watchdog  
0x6B: bq24195 Power Management IC (PMIC)  

---

## GPS / Global Navigation Satellite System Support

The GNSS (Global Navigation Satellite System) library (particle-som-gnss v1.0.0) is designed for Particle cellular modems with built-in GNSS support, including the M-SOM platform. It provides a simple interface for accessing GNSS data on Particle devices. The library is accessed in code via location.h. Source at https://github.com/particle-iot/particle-som-gnss.

The GPS functionality on the Particle Muon continues to work even if cellular is turned off, as the GNSS module operates independently from the cellular modem. Communication is via UART (Serial2 on pins D24/D25).

Location information is included in the INFO feature output as:  
lat, lon, alt (altitude), sat (satellites in use)

Obtaining location information may take some time, with a timeout set at 90 seconds.

