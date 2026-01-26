# Particle Board Factory Reset
[←Top](../README.md)<BR>

## Factory Reset

This mode is almost never useful, for the following reasons:

- The **M-SoM**, **P2**, **Photon 2**, **P1**, and **Photon 1** do not support this feature.  
- **Gen 3** devices (**Argon**, **Boron**) do not have a factory user firmware backup image installed.  
- Even when present, it only provides a user firmware image — **not** a full Device OS install — so it doesn’t truly restore factory defaults.

### How to Perform a Factory Reset

1. Hold down the **MODE** button.  
2. Tap the **RESET** button while continuing to hold down **MODE**.  
3. When the status LED blinks **magenta**, continue holding **MODE**.  
4. When the status LED blinks **yellow**, continue holding **MODE**.  
5. When the status LED blinks **white**, release **MODE**.

### What This Will Do

- Restore the factory backup user firmware (if present).  
- Clear mesh credentials (for Device OS 1.5.2 and earlier).  
- **Boron:** Clear any saved APN and revert to the internal SIM.  
- **Argon:** Clear Wi-Fi credentials.  
- **Ethernet:** Clear the “using Ethernet” flag.  
- Clear the setup complete flag, forcing setup mode again.
