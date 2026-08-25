# 3D-Printed Automatic Weather Stations (3D-PAWS) Particle Full Station Code

## Description

This software is supported on a Particle Boron, Argon band Muon boards. It provides full weather station features. This document supports Version 40 and after. See [README_v39.md](docs/README_v39.md) for prior releases. 

For building a station see [Building 3D-PAWS](https://www.comet.ucar.edu/build-3d-paws).

Major changes as of Version 49. 
- When the configuration file is read and if entry "aqs" is not found in the file; a configuration migration takes place. The .TXT files are read in and a new CONFIG.TXT file is written to the SD card. Then the same file is written to non volatile memory.  After which, if the SD card is not detected, the non volatile config is used.

- Cloud updates are disabled by default. When a firmware update is configured on the Particle Device Console. The device is notified that there is a update pending. It then stops taking observations, update rain totals in non volotile memory, sends any cached observations, enables updates, disconnects from Particle. Then reconnects to Particle. At which point Particle Cloud services sees the device has updates enabled and will start downloading the update. If nothing happens for 5 minutes, the device will reboot. If the device is updated, the device reboots as a result. Controling the update process protects from unexpected reboot corruption of files are being written to the SD card.

- #### !!! After this release do not use the  "Flash now" option on the Particle Device Console. Let the update happen at its own pace to avoid corruption of the SD card from the update reboot.

- Sensor Changes
    - DFRobot SEN0562 - BH1750 LUX Added. Reports as bhlx.
    - TF02Pro TOF Distance Added. Reports as t02d, t02s, t02dt.
    - Removed VEML7700 and HIH8000 Sensors.

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

### [INFO - Station Information](docs/INFO.md)
- Information about Particle message type INFO and INFO.TXT file.

### [LoRa Remote Relay](docs/LoRaRelay.md)
- Receive LoRa messages for LoRa Remote devices. Transmit the message to Particle next transmission period.

### [Muon Board Support](docs/Muon.md)
- Particle Muon Board Support.

### [OLED Display](docs/OLED_Display.md)
- If an OLED display is detected at startup, messages are also sent to this display. Console jumper wire is not needed.

### [Particle DoAction](docs/DoAction.md)
- Send commands to Particle device form Particle Web Console

### [Particle Board Factory Reset](docs/FactoryReset.md)

### [Particle Web Console](docs/ParticleWebConsole.md)
- Features and Setup of the Particle Web Console

### [Pin Mappings - Argon/Boron](docs/ArgonBoronPins.md)

### [SD Card Information](docs/SD.md)
- Information on what is stored on the SD card.

### [Sensor Information](docs/Sensors.md)
- Information about supported Sensors

### [Serial Monitor](docs/SerialMonitor.md)
- Adding a jumper wire between Particle pin D8 (Boron & Argon), A2 (Muon) and ground will enable serial text output on the USB connector at boot time.

### [Station Monitor](docs/StationMonitor.md)
- Upon setup completion.  If the jumper wire is in place then the Station Monitor will run instead of the main work loop.  

### [System Health Bits (hth)](docs/SystemHealthBits.md)
- A health bit register. Where each bit corresponds to a system state or sensor.

### [Voltaic Battery Voltage](docs/VoltaicBatteryVoltage.md)
- The Voltaic Battery provides a battery cell voltage breakout on the USB-C port.

### [WatchDog Board](docs/WatchDog.md)
- The WatchDog is a external device that can turn off power to your weather station microcontroller and sensors. 


