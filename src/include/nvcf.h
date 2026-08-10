/*
 * ======================================================================================================================
 *  nvcf.h - Non Volitile Configuration File Definations
 * 
 * This solution used the Particle’s file-system API, which provides a POSIX-style open() interface 
 * for LittleFS-backed storage.
 * https://docs.particle.io/reference/device-os/file-system/
 * ======================================================================================================================
 */

/* NV CONFIG.TXT
 * ======================================================================================================================
aqs=0                        // 0 = fullstation, 1 = air quality
nowind=0
rg1_enable=0
op1=0
op2=0
op1d5m=0                     // 0= 10m 1=5m Sensor
elevation=0                  // Station elevation. File contents is elevation in meters
rtro=0                       // H(:MM) - valid values are where H = (0-23) with optional ":" and MM = (00,15,30,45)
rcdt=79200                   // Reboot Countdown Timer Seconds
txi=15                       // [5,10,15]
obi=1                        // [1,5,10,15]

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

lat_deg=40.02
lon_deg=-105.26

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

/* SIM.TXT
 Since setting up an exterm sim is a one time thing, there is no support in the nv_cfg file

 sim=AUP,apn,username,passwd       // Option 1: Connect to a cellular network using APN, USERNAME and PASSWORD
        UP,username,password       // Option 2: Connect to a cellular network using USERNAME and PASSWORD
        APN,apn                    // Option 3: Connect to a cellular network using only APN
*/

/* WIFI.TXT
Since WiFi is a one time set and a check there after if the file exists, there is no support in the nv_cfg file

wifi=AuthType,ssid,password
*/

#include "include/evt.h"

/*
 * ======================================================================================================================
 *  Define Global Configuration File Variables
 * ======================================================================================================================
 */
#define DEFAULT_OBS_INTERVAL            1        // 1 minute
#define DEFAULT_OBS_TRANSMIT_INTERVAL   15       // Transmit observations every N minutes Set to 15 for 15min Transmits
#define DEFAULT_REBOOT_COUNTDOWN_TIMER  79200    // Set to 0 to disable feature, approx every 22 hours

 typedef struct {
    int     aqs       = 0;
    int     wind      = 1;
    int     rg1       = 1;
    int     op1       = 0;
    int     op1d5m    = 0;
    int     op2       = 0;
    
    int     rtro_hour   = 0;
    int     rtro_minute = 0;

    int     elevation = 0;
    float   lat_deg = 0.0f;
    float   lon_deg = 0.0f;

    int     rcdt      = DEFAULT_REBOOT_COUNTDOWN_TIMER;
    int     txi       = DEFAULT_OBS_TRANSMIT_INTERVAL;
    int     obi       = DEFAULT_OBS_INTERVAL;

    String  aes_pkey;
    long    aes_myiv;
    int     lora_unitid   = 1;
    int     lora_txpower  = 5;
    int     lora_freq     = 915;

#ifdef ENABLE_Evapotranspiration
    /*
      Albedo, in the context of evapotranspiration, is the fraction of incoming solar radiation that is reflected by the 
      surface back to the atmosphere. It is a dimensionless value ranging from 0 (no reflection, total absorption) to 
      1 (total reflection). For reference crop evapotranspiration calculations, a typical albedo value around 0.23 is used, 
      representing the canopy reflection coefficient of the reference surface like grass. Albedo influences the net 
      radiation available for evapotranspiration because reflected radiation is not available for heating or 
      vaporizing water.
    */
    float   albedo          = 0.23f;

    /*
      Crop coefficient (crop_kc) is a dimensionless factor used in evapotranspiration (ET) calculations that represents the 
      ratio of the evapotranspiration of a specific crop (ETc) to the reference evapotranspiration (ETo), typically for a 
      well-watered reference crop like grass or alfalfa. The crop coefficient incorporates the effects of the crop type, 
      growth stage, and canopy characteristics on water use. It adjusts the reference ET to reflect the actual water use 
      of the crop, including both transpiration by the crop and evaporation from the soil. Crop_kc varies with crop type 
      and growth stage, increasing as the crop canopy develops and then decreasing at maturity.​

      A crop coefficient (crop_kc) value of 1.0 represents a situation where the evapotranspiration of the crop (ETc) is 
      equal to the reference evapotranspiration (ETo). This means the crop is using water at the same rate as the reference 
      crop, typically well-watered grass or alfalfa, which serves as a standard baseline. In practical terms, 
      a crop_kc of 1.0 indicates full crop canopy coverage with optimal growth and water use, neither less nor more than 
      the reference crop's evapotranspiration demand.

      Values less than 1.0 indicate less water use than the reference crop, possibly due to less canopy cover or drought 
      stress, whereas values greater than 1.0 show the crop is using more water than the reference, often due to factors 
      like crop type or growing conditions that increase water 
    */
    float   crop_kc         = 1.0f;

    // Used for Irradiance Calibration
    float   sr_cal          = 75.0f;
    float   sr_dark_offset  = 0.0f;
#endif
} SCV; // System Configuration Variables


// Extern variables
extern SCV scv;

// Function prototypes
bool nv_loadConfig();
bool nv_saveConfig();
void nv_printCfg();
bool nv_deleteConfigFiles();
bool nv_deleteConfigIfCnvFileExists();