# Argon Board Support
[← Back to Top](../README.md)

---

**Particle Argon Datasheet:**  
[Particle Argon Datasheet (official)](https://docs.particle.io/reference/datasheets/wi-fi/argon-datasheet)

---

## Wi-Fi Setup

When setting up an Argon for the first time, use the Particle mobile app to pair with the Argon board.  
This process configures the Wi-Fi credentials, which are saved to non-volatile memory.

- If the initial setup is complete, setup is finished.  
- To change the saved Wi-Fi credentials later, you can:  
  1. Use the Particle mobile app, **or**  
  2. Set configuration on the SD card with the new Wi-Fi information.

---
### Wi-Fi Credentials via SD Card
With the Release of version 49. WiFi configuration moved from SD file WIFI.TXT to CONFIG.TXT. With the below variables.

- wifi_auth=
- wifi_ssid=
- wifi_pw=

### Wi-Fi Credentials via SD Card (Prior to Version 49)

At the **top level of the SD card**, create a file named `WIFI.TXT`.

- The file should have **one line** with **three comma-separated items**:  

    AuthType,ssid,password

    WPA2,MyHomeWifi,Pa$$w0rD!


#### AuthType Options
- `UNSEC`  
- `WEP`  
- `WPA`  
- `WPA2`  
- `WPA_ENTERPRISE`  
- `WPA2_ENTERPRISE`  

---

### Serial Console Output

#### When Wi-Fi info is successfully read:
WIFI:Open  
WIFI:ATYPE[WPA2]  
WIFI:SSID[NotMySSID]  
WIFI:PW[NotMyPassword]  
WIFI:Credentials Cleared  
WIFI:Credentials Set [WPA2]

#### When Wi-Fi file WIFI.TXT not found:
WIFI:NOFILE USING NVAUTH

**Note:** If Wi-Fi credentials cannot be obtained from the SD card, the Argon uses the credentials stored in **Particle's non-volatile memory**.

---

## Pin Mappings
- [Argon Pin Mappings](ArgonBoronPins.md)
