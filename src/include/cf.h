/*
 * ======================================================================================================================
 *  cf.cpp - Configuration File Functions
 * ======================================================================================================================
 */

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

# Replace with your sensor's calibration constant (µV per W/m² from the label/certificate)
# Check the calibration sticker on your SP Lite2 — it will say something like: Sensitivity: 74.8 µV per W/m²
sr_cal=75.0 

# With the sensor covered, log the raw readings, set that as dark offset.
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
extern float cf_sr_cal;
extern float cf_sr_dark_offset;

// Function prototypes
void SD_ReadConfigFile();
