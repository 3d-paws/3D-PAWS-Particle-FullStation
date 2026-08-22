/*
 * ======================================================================================================================
 *  nvcf.h - Non Volitile Configuration File Definations
 * 
 * This solution used the Particle’s file-system API, which provides a POSIX-style open() interface 
 * for LittleFS-backed storage.
 * https://docs.particle.io/reference/device-os/file-system/
 * ======================================================================================================================
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

// The below shows what is in the CONFIG.TXT file
 typedef struct {
    int     aqs       = 0;  // 0=Full Station, 1=Aquality Station
    int     wind      = 1;  // 0=Disabled, 1=Enabled
    int     rg1       = 1;  // 0=Disabled, 1=Enabled
    int     op1       = 0;  // 0=Null, 1=Distance, 2=Rain (rg2), 3=Raw
    int     op1d5m    = 0;  // 0=10m Distance, 1=5m Distance Sensor
    int     op2       = 0;  // 0=Null, 1=Raw, 2=Voltaic
    
    int     rtro_hour   = 0;  // Rain Total Rollover Hour 0-23
    int     rtro_minute = 0;  // Rain Total Rollover Minute 0,15,30,45

    // Used for MSLP and setable via Particle console DoAction
    int     elevation = 0;    // Station Elevation in m

    float   lat_deg = 0.0f;
    float   lon_deg = 0.0f;

    int     rcdt      = DEFAULT_REBOOT_COUNTDOWN_TIMER;
    int     txi       = DEFAULT_OBS_TRANSMIT_INTERVAL;
    int     obi       = DEFAULT_OBS_INTERVAL;

    // Private Key - 128 bits (16 bytes of ASCII characters)
    String  aes_pkey;

    // Initialization Vector must be and always will be 128 bits (16 bytes.)
    // The real iv is actually myiv repeated twice
    // 1234567 -> 0x12D687 = 0x00 0x12 0xD6 0x87 0x00 0x12 0xD6 0x87 
    long    aes_myiv;

    // This unit's LoRa ID for Receiving and Sending Messages
    int     lora_unitid   = 1;

    // You can set transmitter power from 5 to 23 dBm
    int     lora_txpower  = 5;

    // Valid entries are 433, 866, 915
    int     lora_freq     = 915;

    String  wifi_auth;    // WEP WPA WPA2 UNSEC, Not used for MUON
    String  wifi_ssid;
    String  wifi_pw;

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

    // Replace with your Irradiance sensor's calibration constant (µV per W/m² from the label/certificate)
    // Check the calibration sticker on your SP Lite2 — it will say something like: Sensitivity: 74.8 µV per W/m²
    float   sr_cal          = 75.0f;

    // With the Irradiance sensor covered, log the raw readings, set that as dark offset.
    float   sr_dark_offset  = 0.0f;
#endif
} SCV; // System Configuration Variables

// Extern variables
extern SCV scv;
extern bool nv_config_enabled;

// Function prototypes
bool nv_loadConfig();
bool nv_saveConfig();
bool sd_loadConfig();
bool sd_saveConfig();
bool saveConfig();
void nv_printCfg();
bool nv_deleteConfigFiles();
bool nv_deleteConfigIfCnvFileExists();