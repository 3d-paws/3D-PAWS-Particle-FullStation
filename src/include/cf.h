/*
 * ======================================================================================================================
 *  cf.cpp - Configuration File Functions
 * ======================================================================================================================
 */
#include "include/evt.h"

/* 
 * ======================================================================================================================
#
# CONFIG.TXT
#
# Line Length is limited to 63 characters
#12345678901234567890123456789012345678901234567890123456789012

# Private Key - 128 bits (16 bytes of ASCII characters)
aes_pkey=10FE2D3C4B5A6978

# Initialization Vector must be and always will be 128 bits (16 bytes.)
# The real iv is actually myiv repeated twice
# 1234567 -> 0x12D687 = 0x00 0x12 0xD6 0x87 0x00 0x12 0xD6 0x87 
aes_myiv=1234567

# This unit's LoRa ID for Receiving and Sending Messages
lora_unitid=1

# You can set transmitter power from 5 to 23 dBm
lora_txpower=5

# Valid entries are 433, 866, 915
lora_freq=915


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
* ======================================================================================================================
*/

/*
 * ======================================================================================================================
 *  Define Global Configuration File Variables
 * ======================================================================================================================
 */
#define CF_NAME           "CONFIG.TXT"
#define KEY_MAX_LENGTH    30                // Config File Key Length
#define VALUE_MAX_LENGTH  30                // Config File Value Length
#define LINE_MAX_LENGTH   VALUE_MAX_LENGTH+KEY_MAX_LENGTH+3   // =, CR, LF 

// Extern variables
extern char *cf_aes_pkey;
extern long cf_aes_myiv;
extern int cf_lora_unitid;
extern int cf_lora_txpower;
extern int cf_lora_freq;

extern int cf_elevation;

#ifdef ENABLE_Evapotranspiration
// Used for Evapotranspiration
extern float cf_lat_deg;
extern float cf_lon_deg;

 /*
  * ======================================================================================================================
  * Crop coefficient (crop_kc) is a dimensionless factor used in evapotranspiration (ET) calculations that represents the 
  * ratio of the evapotranspiration of a specific crop (ETc) to the reference evapotranspiration (ETo), typically for a 
  * well-watered reference crop like grass or alfalfa. The crop coefficient incorporates the effects of the crop type, 
  * growth stage, and canopy characteristics on water use. It adjusts the reference ET to reflect the actual water use 
  * of the crop, including both transpiration by the crop and evaporation from the soil. Crop_kc varies with crop type 
  * and growth stage, increasing as the crop canopy develops and then decreasing at maturity.​
  * 
  * A crop coefficient (crop_kc) value of 1.0 represents a situation where the evapotranspiration of the crop (ETc) is 
  * equal to the reference evapotranspiration (ETo). This means the crop is using water at the same rate as the reference 
  * crop, typically well-watered grass or alfalfa, which serves as a standard baseline. In practical terms, 
  * a crop_kc of 1.0 indicates full crop canopy coverage with optimal growth and water use, neither less nor more than 
  * the reference crop's evapotranspiration demand.
  *
  * Values less than 1.0 indicate less water use than the reference crop, possibly due to less canopy cover or drought 
  * stress, whereas values greater than 1.0 show the crop is using more water than the reference, often due to factors 
  * like crop type or growing conditions that increase water 
  * ======================================================================================================================
  */
extern float cf_crop_kc;

 /*
  * ======================================================================================================================
  * Albedo, in the context of evapotranspiration, is the fraction of incoming solar radiation that is reflected by the 
  * surface back to the atmosphere. It is a dimensionless value ranging from 0 (no reflection, total absorption) to 
  * 1 (total reflection). For reference crop evapotranspiration calculations, a typical albedo value around 0.23 is used, 
  * representing the canopy reflection coefficient of the reference surface like grass. Albedo influences the net 
  * radiation available for evapotranspiration because reflected radiation is not available for heating or 
  * vaporizing water.
  * ======================================================================================================================
  */
extern float cf_albedo;

// Used for Irradiance Calibration
extern float cf_sr_cal;
extern float cf_sr_dark_offset;
#endif

// Function prototypes
void SD_ReadConfigFile();
void SD_ReadElevationFile();
