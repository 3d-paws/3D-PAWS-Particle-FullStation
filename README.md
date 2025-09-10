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

### [Station Information (INFO)](docs/INFO.md)
Information about Particle message type INFO and INFO.TXT file.

### [Air Quality Station Option and Operation](docs/AirQualityStation.md)
Information about operation as a Air Quality Station (No wind, Rain) and sampling air quality differently.

### [Code Operation Notes](docs/CodeOperation.md)
Information on how the station operates at a code level.

### [System Health Bits (hth)](docs/SystemHealthBits.md)
A health bit register. Where each bit corresponds to a system state or sensor.

### WatchDog Board
 


