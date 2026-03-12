# 3D-PAWS Particle Full Station

Last Updated: 2025-10-08

## Description

This software is supported on a Particle Boron, Argon band Muon boards. It provides full weather station features. Support a Air Quality Sensor mode. This document supports Version 40 and after. See [README_v39.md](docs/README_v39.md) for prior releases.
## Features

### [Air Quality Station Option and Operation](docs/AirQualityStation.md)
- Information about operation as a Air Quality Station (No wind, Rain) and sampling air quality differently.

### [Argon Board Support](docs/Argon.md)
- Particle WiFi Board Support.

### [Boron Board Support](docs/Boron.md)
- Particle Cellular Board Support.

### [Code Operation Notes](docs/CodeOperation.md)
- Information on how the station operates at a code level.

### [Configuration File Example](docs/ConfigFileExample.md)
- Example of CONFIG.TXT

### [Daily Reboot](docs/DailyReboot.md)
- The weather station will perform a daily reboot to clear any issues with the station.

### [Do Action](docs/DoAction.md)
- Commands and settings you can send to the board via Particle Console.

### [Evapotranspiration Information](docs/EVT.md)
- Information about Evapotranspiration

### [LoRa Remote Relay](docs/LoRaRelay.md)
- Receive LoRa messages for LoRa Remote devices. Transmit the message to Particle next transmission period.

### [Muon Board Support](docs/Muon.md)
- Particle Muon Board Support.

### [OLED Display](docs/OLED_Display.md)
- If an OLED display is detected at startup, messages are also sent to this display. Jumper wire is not needed for activation.

### [Particle DoAction](docs/DoAction.md)
- Send commands to Particle device form Particle Web Console

### [Particle Board Factory Reset](docs/FactoryReset.md)

### [Particle Web Console](docs/ParticleWebConsole.md)
- Features and Setup of the Particle Web Console

### [SD Card Information](docs/SD.md)
- Information on what is stored on the SD card.

### [Sensor Information](docs/Sensors.md)
- Information about supported Sensors

### [Serial Monitor](docs/SerialMonitor.md)
- Adding a jumper wire between Particle pin D8 (Boron & Argon), A2 (Muon) and ground will enable serial text output on the USB connector at boot time.

### [Station Information (INFO)](docs/INFO.md)
- Information about Particle message type INFO and INFO.TXT file.

### [Station Monitor](docs/StationMonitor.md)
- Upon setup completion.  If the jumper wire is in place then the Station Monitor will run instead of the main work loop.  

### [System Health Bits (hth)](docs/SystemHealthBits.md)
- - A health bit register. Where each bit corresponds to a system state or sensor.

### [WatchDog Board](docs/WatchDog.md)
- The WatchDog is a external device that can turn off power to your weather station microcontroller and sensors. 


