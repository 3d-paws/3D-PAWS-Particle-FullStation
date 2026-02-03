/*
 * ======================================================================================================================
 *  eeprom.h - EEPROM Definations
 * ======================================================================================================================
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