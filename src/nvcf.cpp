/*
 * ======================================================================================================================
 *  nvcf.cpp - Non Volitile Configuration File Functions
 * ======================================================================================================================
 */
#include <Particle.h>
#include <fcntl.h>

#include "include/qc.h"
#include "include/support.h"
#include "include/ssbits.h"
#include "include/main.h"
#include "include/sdcard.h"
#include "include/lora.h"
#include "include/output.h"
#include "include/evt.h"
#include "include/sensors.h" 
#include "include/wrda.h"
#include "include/cf.h"
#include "include/nvcf.h"

/*
 * ======================================================================================================================
 * Variables and Data Structures
 * =======================================================================================================================
 */
SCV scv; // System Configuration Variables

/*
 * ======================================================================================================================
 *  Define Global Configuration File Variables
 * ======================================================================================================================
 */
static const char* NV_CONFIG_FILE = "/CONFIG.TXT";
static const char* NV_TEMP_FILE   = "/CONFIG.TMP";


/*
 * ======================================================================================================================
 * Fuction Definations
 * =======================================================================================================================
 */

/*
 * ======================================================================================================================
 * nv_parseLine()
 * =======================================================================================================================
 */
bool nv_parseLine(const String& line) {
    String s = line;
    s.trim();
    if (s.length() == 0) return true;
    if (s.startsWith("#") || s.startsWith("//")) return true;

    int eq = s.indexOf('=');
    if (eq < 1) return false;

    String key = s.substring(0, eq);
    key.trim();
    String val = s.substring(eq + 1);
    val.trim();

    if (key == "aqs") scv.aqs = val.toInt();
    else if (key == "wind") scv.wind = val.toInt();
    else if (key == "rg1") scv.rg1 = val.toInt();
    else if (key == "op1") scv.op1 = val.toInt();
    else if (key == "op1d5m") scv.op1d5m = val.toInt();
    else if (key == "op2") scv.op2 = val.toInt();
    else if (key == "elevation") scv.elevation = val.toInt();
    else if (key == "rtro_hour") scv.rtro_hour = val.toInt();
    else if (key == "rtro_minute") scv.rtro_minute = val.toInt();
    else if (key == "rcdt") scv.rcdt = val.toInt();
    else if (key == "txi") scv.txi = val.toInt();
    else if (key == "obi") scv.obi = val.toInt();
    else if (key == "aes_pkey") scv.aes_pkey = val;
    else if (key == "aes_myiv") scv.aes_myiv = atol(val.c_str());
    else if (key == "lora_unitid") scv.lora_unitid = val.toInt();
    else if (key == "lora_txpower") scv.lora_txpower = val.toInt();
    else if (key == "lora_freq") scv.lora_freq = val.toInt();
#ifdef ENABLE_Evapotranspiration
    else if (key == "lat_deg") scv.lat_deg = val.toFloat();
    else if (key == "lon_deg") scv.lon_deg = val.toFloat();
    else if (key == "albedo") scv.albedo = val.toFloat();
    else if (key == "crop_kc") scv.crop_kc = val.toFloat();
    else if (key == "sr_cal") scv.sr_cal = val.toFloat();
    else if (key == "sr_dark_offset") scv.sr_dark_offset = val.toFloat();
#endif
    else return false;

    return true;
}

/*
 * ======================================================================================================================
 * nv_writeAll()
 * =======================================================================================================================
 */
static ssize_t nv_writeAll(int fd, const char* data, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t n = write(fd, data + total, len - total);
        if (n < 0) return -1;
        total += (size_t)n;
    }
    return (ssize_t)total;
}

/*
 * ======================================================================================================================
 * nv_writeLine()
 * =======================================================================================================================
 */
static bool nv_writeLine(int fd, const char* fmt, ...) {
    char buf[160];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    if (n < 0 || (size_t)n >= sizeof(buf)) return false;
    return nv_writeAll(fd, buf, (size_t)n) == n;
}

/*
 * ======================================================================================================================
 * nv_loadConfig()
 * =======================================================================================================================
 */
bool nv_loadConfig() {
    int fd = open(NV_CONFIG_FILE, O_RDONLY);
    if (fd < 0) {
      sprintf(msgbuf, "NVCF:%s NF, Using Defaults", NV_CONFIG_FILE);
      Output(msgbuf);
      return false;
    }
    sprintf(msgbuf, "NVCF:%s Found", NV_CONFIG_FILE);
    Output(msgbuf);

    String line;
    char ch;

    while (true) {
        ssize_t n = read(fd, &ch, 1);
        if (n <= 0) break;

        if (ch == '\n') {
            nv_parseLine(line);
            line = "";
        } else if (ch != '\r') {
            line += ch;
        }
    }

    if (line.length() > 0) {
        nv_parseLine(line);
    }

    close(fd);
    return true;
}

/*
 * ======================================================================================================================
 * nv_saveConfig()
 * =======================================================================================================================
 */
bool nv_saveConfig() {
    int fd = open(NV_TEMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        return false;
    }

    bool ok = true;
    ok &= nv_writeLine(fd, "aqs=%d\n", scv.aqs);
    ok &= nv_writeLine(fd, "nowind=%d\n", scv.wind);
    ok &= nv_writeLine(fd, "rg1_enable=%d\n", scv.rg1);
    ok &= nv_writeLine(fd, "op1=%d\n", scv.op1);
    ok &= nv_writeLine(fd, "op1d5m=%d\n", scv.op1d5m);
    ok &= nv_writeLine(fd, "op2=%d\n", scv.op2);
    ok &= nv_writeLine(fd, "elevation=%d\n", scv.elevation);
    ok &= nv_writeLine(fd, "rtro_hour=%s\n", scv.rtro_hour);
    ok &= nv_writeLine(fd, "rtro_minute=%s\n", scv.rtro_minute);
    ok &= nv_writeLine(fd, "rcdt=%d\n", scv.rcdt);
    ok &= nv_writeLine(fd, "txi=%d\n", scv.txi);
    ok &= nv_writeLine(fd, "obi=%d\n", scv.obi);
    ok &= nv_writeLine(fd, "aes_pkey=%s\n", scv.aes_pkey.c_str());
    ok &= nv_writeLine(fd, "aes_myiv=%ld\n", scv.aes_myiv);
    ok &= nv_writeLine(fd, "lora_unitid=%d\n", scv.lora_unitid);
    ok &= nv_writeLine(fd, "lora_txpower=%d\n", scv.lora_txpower);
    ok &= nv_writeLine(fd, "lora_freq=%d\n", scv.lora_freq);
#ifdef ENABLE_Evapotranspiration
    ok &= nv_writeLine(fd, "lat_deg=%.6f\n", scv.lat_deg);
    ok &= nv_writeLine(fd, "lon_deg=%.6f\n", scv.lon_deg);
    ok &= nv_writeLine(fd, "albedo=%.3f\n", scv.albedo);
    ok &= nv_writeLine(fd, "crop_kc=%.3f\n", scv.crop_kc);
    ok &= nv_writeLine(fd, "sr_cal=%.3f\n", scv.sr_cal);
    ok &= nv_writeLine(fd, "sr_dark_offset=%.3f\n", scv.sr_dark_offset);
#endif

    fsync(fd);
    close(fd);

    if (!ok) {
        remove(NV_TEMP_FILE);
        return false;
    }

    remove(NV_CONFIG_FILE);
    return rename(NV_TEMP_FILE, NV_CONFIG_FILE) == 0;
}

/*
 * ======================================================================================================================
 * nv_printCfg()
 * =======================================================================================================================
 */
void nv_printCfg() {
    struct NvItem {
        const char* key;
        const char* fmt;
        const void* ptr;
    };

    NvItem items[] = {
        {"aqs", "%d", &scv.aqs},
        {"wind", "%d", &scv.wind},
        {"rg1", "%d", &scv.rg1},
        {"op1", "%d", &scv.op1},
        {"op1d5m", "%d", &scv.op1d5m},
        {"op2", "%d", &scv.op2},
        {"elevation", "%d", &scv.elevation},
        {"rtro_hour", "%d", &scv.rtro_hour},
        {"rtro_minute", "%d", &scv.rtro_minute},
        {"rcdt", "%d", &scv.rcdt},
        {"txi", "%d", &scv.txi},
        {"obi", "%d", &scv.obi},
        {"aes_pkey", "%s", scv.aes_pkey.c_str()},
        {"aes_myiv", "%ld", &scv.aes_myiv},
        {"lora_unitid", "%d", &scv.lora_unitid},
        {"lora_txpower", "%d", &scv.lora_txpower},
        {"lora_freq", "%d", &scv.lora_freq},
#ifdef ENABLE_Evapotranspiration
        {"lat_deg", "%f", &scv.lat_deg},
        {"lon_deg", "%f", &scv.lon_deg},
        {"albedo", "%f", &scv.albedo},
        {"crop_kc", "%f", &scv.crop_kc},
        {"sr_cal", "%f", &scv.sr_cal},
        {"sr_dark_offset", "%f", &scv.sr_dark_offset},
#endif
    };

    Output("NV Settings:");

    for (size_t i = 0; i < sizeof(items) / sizeof(items[0]); i++) {
        if (strcmp(items[i].fmt, "%s") == 0) {
            snprintf(msgbuf, sizeof(msgbuf), "  %s:%s", items[i].key, (const char*)items[i].ptr);
        }
        else if (strcmp(items[i].fmt, "%ld") == 0) {
            snprintf(msgbuf, sizeof(msgbuf), "  %s:%ld", items[i].key, *(const long*)items[i].ptr);
        }
        else if (strcmp(items[i].fmt, "%d") == 0) {
            snprintf(msgbuf, sizeof(msgbuf), "  %s:%d", items[i].key, *(const int*)items[i].ptr);
        }
        else if (strcmp(items[i].fmt, "%f") == 0) {
            snprintf(msgbuf, sizeof(msgbuf), "  %s:%f", items[i].key, *(const float*)items[i].ptr);
        }

        Output(msgbuf);
    }
}

/*
 * ======================================================================================================================
 * nv_deleteConfigFiles()
 * Deletes CONFIG.TXT and CONFIG.TMP when present.
 *
 * Returns true only if both files are either successfully deleted or did not exist.
 * ======================================================================================================================
 */
bool nv_deleteConfigFiles() {
    const char* files[] = {
        NV_CONFIG_FILE,
        NV_TEMP_FILE
    };

    bool success = true;

    for (size_t i = 0; i < (sizeof(files) / sizeof(files[0])); i++) {
        const char* fileName = files[i];

        // unlink() deletes the file. ENOENT means it was already absent.
        if (unlink(fileName) == 0) {
            snprintf(msgbuf, sizeof(msgbuf), "NVCF:%s Deleted", fileName);
            Output(msgbuf);
        }
        else if (errno == ENOENT) {
            snprintf(msgbuf, sizeof(msgbuf), "NVCF:%s Not Found", fileName);
            Output(msgbuf);
        }
        else {
            snprintf(msgbuf, sizeof(msgbuf), "NVCF:%s Delete Failed: %s",
                     fileName, strerror(errno));
            Output(msgbuf);
            success = false;
        }
    }

    return success;
}

/*
 * ======================================================================================================================
 * nv_deleteConfigIfCnvFileExists()
 * If the SD card is available and CNV.TXT exists:
 *   1. Delete CONFIG.TXT and CONFIG.TMP from NV
 *   2. Delete CNV.TXT after successful config deletion
 *
 * Returns true only when CNV.TXT was found and all requested deletes succeeded.
 * ======================================================================================================================
 */
bool nv_deleteConfigIfCnvFileExists() {
    // Do not access the SD filesystem unless it is known to be available.
    if (!SD_exists) {
        Output("CNV:SD Not Available, config files retained");
        return false;
    }

    // CNV.TXT is the trigger file.
    if (access(SD_cnv_file, F_OK) != 0) {
        if (errno == ENOENT) {
            snprintf(msgbuf, sizeof(msgbuf), "CNV:%s Not Found, config files retained", SD_cnv_file);
        }
        else {
            snprintf(msgbuf, sizeof(msgbuf), "CNV:%s Check Failed: %s", SD_cnv_file, strerror(errno));
        }
        Output(msgbuf);
        return false;
    }

    snprintf(msgbuf, sizeof(msgbuf), "CNV:%s Found, deleting config files", SD_cnv_file);
    Output(msgbuf);

    // Keep CNV.TXT in place if deleting the config files failed.
    if (!nv_deleteConfigFiles()) {
        Output("CNV:Config delete failed, retaining CNV.TXT");
        return false;
    }

    // Config files were successfully deleted; remove the trigger file too.
    if (unlink(SD_cnv_file) == 0) {
        snprintf(msgbuf, sizeof(msgbuf), "NVCF:%s Deleted", SD_cnv_file);
        Output(msgbuf);
        Output("CNV: HALT-entering infinite loop");
        while (true) {
            HeartBeat();
            delay (750);
        }
    }

    snprintf(msgbuf, sizeof(msgbuf), "CNV:%s Delete Failed: %s", SD_cnv_file, strerror(errno));
    Output(msgbuf);

    return false;
}