/*
 * ======================================================================================================================
 *  eeprom.h - EEPROM Definations
 * ======================================================================================================================
 */

  /*
    Assume about 100,000 effective write/erase cycles for the flash region used for emulated EEPROM.

    For wear‑leveling purposes on a Particle Boron, the EEPROM_NVM struct is effectively spread over the 
    full 4096‑byte emulated‑EEPROM region that Device‑OS manages as one logical block

    Worst case writing 32 bytes once a minute. So 41,000 writes per year (365 × 24 × 60)

    With Particle’s built‑in wear‑leveling on the Boron, writing your EEPROM_NVM struct once per minute is very 
    likely to last many years—probably far longer than the device’s practical field life.

 */

/*
 * ======================================================================================================================
 *  EEPROM NonVolitileMemory - stores rain totals in persistant memory
 * ======================================================================================================================
 */
typedef struct {
    float    rgt1;       // rain gauge total today
    float    rgp1;       // rain gauge total prior
    float    rgt2;       // rain gauge 2 total today
    float    rgp2;       // rain gauge 2 total prior
    time32_t rgts;       // rain gauge timestamp of last modification
    unsigned long n2sfp; // sd need 2 send file position
    unsigned long checksum;
} EEPROM_NVM;

// Extern variables
extern EEPROM_NVM eeprom;
extern bool eeprom_valid;
extern bool eeprom_exists;

// Function prototype
unsigned long EEPROM_ChecksumCompute();
void EEPROM_ChecksumUpdate();
bool EEPROM_ChecksumValid();
void EEPROM_ClearRainTotals(time32_t current_time);
void EEPROM_ClearRain2Totals();
void EEPROM_Validate();
void EEPROM_UpdateRainTotals(float rgt1, float rgt2);
void EEPROM_SaveUnreportedRain();
void EEPROM_Update();
void EEPROM_Dump();
void EEPROM_Initialize();