# Evapotranspiration Information
[←Top](../README.md)<BR>

## Support Notes 

Evapotranspiration support is a one off and may not stay in the code base. The code for this feature is located in file evt.h and evt.cpp. There are conditional compilation directives `#ifdef` through the main code to wedge the feature in.  It can be compiled out of the code by use of conditional compilation directives. To do so, undefine `ENABLE_Evapotranspiration` at the top of evt.h. 

## Requirements 

To support Evapotranspiration, the station must have the following sensors 

- Irradiance Sensor - via ADS with SP Lite2 Pyranometer from Kipp & Zonen on 0 & 1 ports
- Adafruit SHT31 Sensor for Temperature & Humidity
- Wind Spped support

In addition it needs station elevation (m) See ELEV.TXT. 

Along with several configuration file settings.
```
#
# CONFIG.TXT
#
# Line Length is limited to 63 characters
#12345678901234567890123456789012345678901234567890123456789012

########################################
# Evapotranspiration - evt.cpp and evt.h
########################################
lat_deg=40.02
lon_deg=-105.26
# Set elevation (m) in file ELEV.TXT - Used for MSLP and setable via Particle console DoAction

# Value 0.23 (grass)
albedo=0.23

# Value 1.0 sets to baseline crop grass 
crop_kc=1.0

# Irradiance
# Replace with your Irradiance sensor's calibration constant (µV per W/m² from the label/certificate)
# Check the calibration sticker on your SP Lite2 — it will say something like: Sensitivity: 74.8 µV per W/m²
sr_cal=75.0 

# With the Irradiance sensor covered, log the raw readings, set that as dark offset.
sr_dark_offset=0.0
```

## Operation

Function evt_do() is called once a minute. It will take Minute Sample, On the hour it will compute ET0, ETc, 
report these values as Particle Event Type `ET` with observation tags `et` and `etc`.
 
 ## Notes & assumptions

- Wind height: All stations are at 2 m  so no height correction.
- Soil heat flux (G): FAO-56 hourly guideline used (0.1·Rₙ in daylight, 0.5·Rₙ at night).
- Net longwave (Rnl): Uses hourly approximation with a single T (K) and the clearness term from Rs/Rso.
- Solar time: We apply the equation of time and longitude to approximate solar hour angles (good accuracy for hourly ET₀).
- Units: Output ET₀ is mm·h⁻¹. Summing 24 hours gives daily ET₀ (mm·d⁻¹).
- ETc: If you set crop_kc ≠ 1, we also report ETc = Kc·ET₀.