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
bool EEPROM_TimeToRollOver();
void EEPROM_UpdateRainTotals(float rgt1, float rgt2);
void EEPROM_SaveUnreportedRain();
void EEPROM_Update();
void EEPROM_Dump();
void EEPROM_Initialize();


/* 
QSPI flash chip on the Particle Boron is accessed via the dedicated QSPI (Quad SPI) peripheral of the nRF52840 MCU
https://docs.particle.io/reference/device-os/file-system/

Consider LittleFS uses the the external QSPI flash chip on the Boron

How Particle uses it

Particle splits that 4 MB between several things:
    Device OS / OTA staging: part of the QSPI flash is used as a scratch area for OTA firmware transfers and updates, so the system can safely copy and verify the new image.
    LittleFS‑based file system: about half the QSPI flash (around 2 MB) is reserved for the POSIX‑style file system; that’s where your LittleFS files live.
    User NVM space: the rest of the QSPI flash is used for the User NVM block/stream layer, which is how the emulated‑EEPROM‑style functions are implemented on top of the same physical chip.

Multiple layers (LittleFS, User NVM, OTA staging) all share the same 4 MB QSPI chip, but each gets its own reserved region

You never “see” the raw QSPI chip; you touch it only through LittleFS, User NVM, or special‑mode DFU‑over‑USB

On the Boron (and other Gen‑3 Particle devices), Particle does allocate a LittleFS‑based file system by default in the external QSPI flash, and that partition is enabled and ready to use in normal Device OS builds.

The QSPI flash is pre‑partitioned at manufacture, and one of the partitions is a 2 MB LittleFS area starting at 0x00000000 in the QSPI layout.

Particle’s OS uses that same LittleFS area internally for:
    emulated EEPROM (the EEPROM object is implemented as a file in LittleFS),
    various config and device‑state files,
    OTA‑related scratch and staging when needed.

For your application, you can also open your own files in that same LittleFS partition via the standard LittleFS API (LittleFS.open(...), File.write(...), etc.), without having to “enable” or create the file system yourself.

-- 

    On Gen‑3 devices (Boron, Argon, Xenon), Particle’s EEPROM is not written directly to a fixed offset in flash; instead it is stored as a small file within the LittleFS‑backed 2 MB QSPI file system.

    Every time you call EEPROM.put(), Device OS:

        Writes a new copy of the emulated‑EEPROM data to fresh LittleFS sectors,
        Letting the old sectors be garbage‑collected,
        Relying on LittleFS’s own wear‑leveling over all available free sectors.

So:

    The same wear‑leveling logic that protects your own LittleFS files (e.g., "rainlog.bin") also protects the internal EEPROM‑emulation file.
    The only difference is abstraction level: you see EEPROM.put(eeprom_address, eeprom);, but behind the scenes that’s just a LittleFS file being managed with the same sector‑based wear‑leveling.


--

If SD card is there at boot
 1) Look for the ADD_/DEL_ on LittleFS file system and perform action on SD card 
      This means prior to boot the SD card was not there and we had performed DoAction functions.
 2) SD card is considered master at this point, make LittleFS match the SD card
 3) Boot continues and looks for config files
 4) DoAction commands add/delete files fron LittleFS and SD card

If SD card is not there at boot
 1) Read configuration files from LittleFS
 2) DoAction commands will add/delete files from LittleFS and also add an additional files like ADD_NOWIND.TXT for example.
       Later when the SD Card is found at boot the ADD_/DEL_ actions are performed on the SD card.

At boot after the state of the SD in known. If found, we call function SD_LittleFS_Sync() 
All functions checking for config files will look on the SD card first if online. If not then looks at LittleFS for the file.

--

#include "Particle.h"
#include "LittleFS.h"

File eepromFile;

typedef struct {
    float    rgt1;
    float    rgp1;
    float    rgt2;
    float    rgp2;
    uint32_t rgts;
    unsigned long n2sfp;
    unsigned long checksum;
} EEPROM_NVM;

EEPROM_NVM eeprom;

void save_to_littlefs() {

    // Power‑down during a write can be an issue
    // LittleFS and User NVM both have internal checksums or CRCs, 
    // so a subsequent read will usually detect the corruption and 
    // return invalid or zeroed data, not silently “wrong” data.
    eepromFile = LittleFS.open("eeprom.bin", FILE_WRITE);
    if (eepromFile) {
        eepromFile.write((uint8_t*)&eeprom, sizeof(eeprom));
        eepromFile.close();
    }
}

void load_from_littlefs() {
    eepromFile = LittleFS.open("eeprom.bin", FILE_READ);
    if (eepromFile) {
        size_t r = eepromFile.read((uint8_t*)&eeprom, sizeof(eeprom));
        eepromFile.close();
        if (r != sizeof(eeprom)) {
            // file missing or truncated; init defaults
            memset(&eeprom, 0, sizeof(eeprom));
        }
    }
}

-----------------------------

We could do away with individual files and do a JSON config file.

// Example file
{
  "ssid": "mywifi",
  "password": "secret123",
  "threshold": 25.5,
  "enabled": true,
  "sensor_interval": 5000,
  "device_id": "BORON123",
  "ota_enabled": false
}


SEE https://github.com/rickkas7/JsonParserGeneratorRK


STARTUP(System.enableFeature(FEATURE_FILESYSTEM));
#include "Particle.h"
#include "Particle-JSON.h"  // For JsonParserGeneratorRK

// Write example
JsonParserGeneratorRK parser;
int fd = open("/config.json", O_RDWR | O_CREAT | O_TRUNC);
if (fd != -1) {
    JsonWriterRK writer(fd);
    writer.beginObject();
    writer.name("ssid").value("mywifi");
    writer.name("threshold").value(25.5f);
    writer.name("enabled").value(true);
    writer.endObject();
    fsync(fd);
    close(fd);
}

// Read Example
int fd = open("/config.json", O_RDONLY);
if (fd != -1) {
    struct stat st;
    fstat(fd, &st);
    char buf[st.st_size + 1];
    read(fd, buf, st.st_size);
    buf[st.st_size] = 0;
    close(fd);
    
    if (parser.parse(buf)) {
        String ssid = parser.getString("ssid");      // "mywifi"
        float thresh = parser.getFloat("threshold"); // 25.5
        bool enabled = parser.getBool("enabled");    // true
    }
}


*/