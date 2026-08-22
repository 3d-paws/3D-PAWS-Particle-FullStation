/*
 * ======================================================================================================================
 *  cf.cpp - Configuration File Functions
 * ======================================================================================================================
 */
#include <Particle.h>
#include <SdFat.h>

#include "include/qc.h"
#include "include/support.h"
#include "include/ssbits.h"
#include "include/main.h"
#include "include/sdcard.h"
#include "include/lora.h"
#include "include/output.h"
#include "include/evt.h"
#include "include/eeprom.h"
#include "include/wrda.h"
#include "include/nvcf.h"
#include "include/cf.h"

/*
 * ======================================================================================================================
 * Variables and Data Structures
 * =======================================================================================================================
 */

/*
 * ======================================================================================================================
 *  Define Global Configuration File Variables
 * ======================================================================================================================
 */

/*
 * ======================================================================================================================
 * Fuction Definations
 * =======================================================================================================================
 */

/* 
 * =======================================================================================================================
 * SD_ReadElevationFile()
 * =======================================================================================================================
 */
void SD_ReadElevationFile() {
  if (SD_exists && SD.exists(SD_ELEV_FILE)) {
    File elevfile = SD.open(SD_ELEV_FILE, FILE_READ);
    if (elevfile) {
      char buf[16];  // Enough for an int including sign and null terminator
      size_t idx = 0;
      while (elevfile.available() && idx < sizeof(buf) - 1) {
        char c = elevfile.read();
        if (c == '\n' || c == '\r') {
          break;
        }
        buf[idx++] = c;
      }
      buf[idx] = '\0';  // Null-terminate the string
      elevfile.close();

      if (isValidNumberString(buf)) {
        int tmpElev = atoi(buf);  // convert to int

        // Quality check
        if (tmpElev >= QC_MIN_ELEV && tmpElev <= QC_MAX_ELEV) {
          scv.elevation = tmpElev;
          sprintf(msgbuf, "ELEV:%d", tmpElev);
        } else {
          sprintf(msgbuf, "ELEV:QCERR %d", tmpElev);
        }
      }
      else {
        sprintf(msgbuf, "ELEV:!NUMERIC");
      }
    } else {
      sprintf(msgbuf, "ELEV:OPENERR %s", SD_ELEV_FILE);
    }
  }
  else {
    sprintf(msgbuf, "ELEV:NO %s", SD_ELEV_FILE);
  }
  Output(msgbuf);
}

/* 
 * =======================================================================================================================
 * SD_Read_RTRO_File()
 * =======================================================================================================================
 */
void SD_Read_RTRO_File() {
  scv.rtro_hour = 0;
  scv.rtro_minute = 0;

  if (SD_exists && SD.exists(SD_RTRO_FILE)) {
    File rtutcfile = SD.open(SD_RTRO_FILE, FILE_READ);
    if (rtutcfile) {
      char buf[16];  // Enough for an int including sign and null terminator
      int hour = 0;
      int minute = 0;
      bool valid = false;
      size_t idx = 0;

      while (rtutcfile.available() && idx < sizeof(buf) - 1) {
        char c = rtutcfile.read();
        if (c == '\n' || c == '\r') {
          break;
        }
        buf[idx++] = c;
      }
      buf[idx] = '\0';  // Null-terminate the string
      rtutcfile.close();

      int parsed = sscanf(buf, "%d:%d", &hour, &minute);

      if (parsed == 2) {
        // H:MM format - validate quarter-hour
        if ((hour >= 0 && hour <= 23) && (minute == 0 || minute == 15 || minute == 30 || minute == 45)) {
          valid=true;
        }
      } 
      else if (parsed == 1) {
        // Just H format (minute = 0)
        if (hour >= 0 && hour <= 23) {
          valid=true;
        }
      } 

      if (valid) {
        scv.rtro_hour = hour;
        scv.rtro_minute = minute;
        sprintf(msgbuf, "RTRO:%d:%02d", scv.rtro_hour, scv.rtro_minute);
      }
      else {
        sprintf(msgbuf, "RTRO:0 INVALID");
      }
    } else {
      sprintf(msgbuf, "RTRO:0 OPENERR %s", SD_RTRO_FILE);
    }
  }
  else {
    sprintf(msgbuf, "RTRO:0 NF %s", SD_RTRO_FILE);
  }
  Output(msgbuf);
}

/* 
 * =======================================================================================================================
 * SD_CheckNoWindFile()
 * =======================================================================================================================
 */
void SD_CheckNoWindFile() {

  if (SD_exists) {
    if (SD.exists(SD_NOWIND_FILE)) {
      Output ("WIND: Disabled");
      scv.wind=false;
    }
    else {
      Output ("WIND: Enabled");
    }
  }
  else {
    Output("WIND: Enabled, SD NF"); 
  }     
}

/* 
 * =======================================================================================================================
 * SD_CheckNoRainFile()
 * =======================================================================================================================
 */
void SD_CheckNoRainFile() {

  if (SD_exists) {
    if (SD.exists(SD_NORAIN_FILE)) {
      Output ("RAIN: Disabled");
      scv.rg1=false;
    }
    else {
      Output ("RAIN: Enabled");
    }
  }
  else {
    Output("RAIN: Enabled, SD NF"); 
  }     
}

/* 
 *=======================================================================================================================
 * SD_CheckAQSFile() - Check SD Card for file to determine if we are a Air Quality Station
 *=======================================================================================================================
 */
void SD_CheckAQSFile() {

  if (SD_exists) {
    if (SD.exists(SD_OPTAQS_FILE)) {
      Output ("AQS: Enabled");
      scv.aqs = true;
    }
    else {
      Output ("AQS: NF");
      scv.aqs = false;
    }
  }
  else {
      Output ("AQS: Disabled, SD NF");
      scv.aqs = false;    
  }
}

/* 
 *=======================================================================================================================
 * SD_CheckOP1Files() - Check Files related to Option Pin 1 and set scv.op1
 *=======================================================================================================================
 */
void SD_CheckOP1Files() {
  if (SD_exists) {
    if (SD.exists(SD_OP1_DIST_FILE)) {
      Output ("OP1=DIST");
      scv.op1 = OP1_STATE_DISTANCE;
    }
    else if (SD.exists(SD_OP1_RAIN_FILE)) {
      Output ("OP1=RAIN");
      scv.op1 = OP1_STATE_RAIN;
    }
    else if (SD.exists(SD_OP1_RAW_FILE)) {
      Output ("OP1=RAW");
      scv.op1 = OP1_STATE_RAW;
    }
    else {
      Output ("OP1=NULL");
      scv.op1 = OP1_STATE_NULL;
    }
  }
  else {
    Output ("OP1=NULL,SD NF");
    scv.op1 = OP1_STATE_NULL;
  }
}

/* 
 *=======================================================================================================================
 * SD_CheckOP15MFile() - Check SD Card for file to determine if we are a Air Quality Station
 *=======================================================================================================================
 */
void SD_CheckOP15MFile() {

  if (SD_exists) {
    if (SD.exists(SD_OP1_D5M_FILE)) {
      Output ("OP15M: Enabled");
      scv.op1d5m = true;
    }
    else {
      Output ("OP15M: NF");
      scv.op1d5m = false;
    }
  }
  else {
      Output ("OP15M: Disabled, SD NF");
      scv.op1d5m = false;    
  }
}

/* 
 *=======================================================================================================================
 * SD_CheckOP2File() - Check Files related to Option Pin 2 and set scv.op2
 *=======================================================================================================================
 */
void SD_CheckOP2Files() {
  if (SD_exists) {
    if (SD.exists(SD_OP2_RAW_FILE)) {
      Output ("OP2=RAW");
      scv.op2 = OP2_STATE_RAW;
    }
    else if (SD.exists(SD_OP2_VBV_FILE)) {
      Output ("OP2=VBV");
      scv.op2 = OP2_STATE_VOLTAIC;
    }
    else {
      Output ("OP2=NULL");
      scv.op2 = OP2_STATE_NULL;
    }
  }
  else {
    Output ("OP2=NULL,SD NF");
    scv.op2 = OP2_STATE_NULL;
  }
}

/* 
 *=======================================================================================================================
 * SD_CheckN2SandSetSSB() - Check Need 2 Send file and set SystemStatusBits if found
 *=======================================================================================================================
 */
void SD_CheckN2SandSetSSB() {
  if (SD_exists) {
    if (SD.exists(SD_n2s_file)) {
      SystemStatusBits |= SSB_N2S; // Turn on Bit to Report if we have Need to Send Observations
      Output("N2S:Exists");
    }
  }
}

/* 
 *=======================================================================================================================
 * SD_CheckOBITXIFiles() - Observation Interval and Transmit Interval Initialize
 *=======================================================================================================================
 */
void SD_CheckOBITXIFiles() {
  if (SD_exists) {
    if (SD.exists(SD_TX5M_FILE)) {
      Output ("TXI5M Found");
      scv.txi = 5;
      SD_RemoveFile (SD_TX10M_FILE);
      SD_RemoveFile (SD_OB5M_FILE);
      SD_RemoveFile (SD_OB10M_FILE);
      SD_RemoveFile (SD_OB15M_FILE);
    }
    else if (SD.exists(SD_TX10M_FILE)) {
      Output ("TXI10M Found");
      scv.txi = 10;
      SD_RemoveFile (SD_OB5M_FILE);
      SD_RemoveFile (SD_OB10M_FILE);
      SD_RemoveFile (SD_OB15M_FILE);
    }
    else if (SD.exists(SD_OB5M_FILE)) {
      Output ("OBI5M Found");
      scv.obi = scv.txi = 5;
      SD_RemoveFile (SD_OB10M_FILE);
      SD_RemoveFile (SD_OB15M_FILE);
    }
    else if (SD.exists(SD_OB10M_FILE)) {
      Output ("OBI10M Found");
      scv.obi = scv.txi = 10;
      SD_RemoveFile (SD_OB15M_FILE);
    }
    else if (SD.exists(SD_OB15M_FILE)) {
      Output ("OBI15M Found");
      scv.obi = scv.txi = 15;
    }
    else {
      Output ("OBITXI: NF, Using Defaults");
      scv.obi = DEFAULT_OBS_INTERVAL;
      scv.txi = DEFAULT_OBS_TRANSMIT_INTERVAL;
    }
  }
}

/*
 * ======================================================================================================================
 * SD_Read_WiFiFile() - Read WIFI.TXT file      
 * ======================================================================================================================
 */
void SD_Read_WiFiFile() {
  File fp;
  int i=0;
  char *p, *auth, *ssid, *pw;
  char ch, buf[128];

  if (SD_exists) {
    // Test for file WIFI.TXT
    if (SD.exists(SD_wifi_file)) {
      fp = SD.open(SD_wifi_file, FILE_READ); // Open the file for reading, starting at the beginning of the file.

      if (fp) {
        // Deal with too small or too big of file
        if (fp.size()<=7 || fp.size()>127) {
          fp.close();
          Output ("WIFI:Invalid SZ");
        }
        else {
          Output ("WIFI:Open");
          // Read one line from file
          while (fp.available() && (i < (sizeof(buf) - 1))) {
            ch = fp.read();

            // sprintf (msgbuf, "%02X : %c", ch, ch);
            // Output (msgbuf);

            if ((ch == 0x0A) || (ch == 0x0D) ) {  // newline or linefeed
              break;
            }
            else {
              buf[i++] = ch;
            }
          }
          fp.close();

          // At this point we have encountered EOF, CR, or LF
          // Now we need to terminate array with a null to make it a string
          buf[i] = '\0';

          // Parse string for the following
          //   WIFI ssid password
          p = buf;
          auth = strtok_r(p, ",", &p);
          ssid = strtok_r(p, ",", &p);
          pw   = strtok_r(p, ",", &p);

          scv.wifi_auth = auth ? auth : "";
          scv.wifi_ssid = ssid ? ssid : "";
          scv.wifi_pw   = pw   ? pw   : "";
        }
      }
      else {
        sprintf (msgbuf, "WIFI:Open[%s] Err", SD_wifi_file);          
        Output(msgbuf);
        Output ("WIFI:USING NVAUTH");
      }
    } 
    else {
      Output ("WIFI:NOFILE USING NVAUTH");
    }
  } // SD enabled
  else {
    Output ("WIFI:NOSD USING NVAUTH");
  }
}

/* 
 *=======================================================================================================================
 * SD_GetSystemVariables() - Read in main and individual configuration files and set system variables
 *=======================================================================================================================
 */
void SD_GetSystemVariables() {
  if (SD_exists) {

    SD_CheckN2SandSetSSB();

    sd_loadConfig();
    if (!nv_config_enabled) {
      // Read old individual files to get the system config variables

      // Rename A4 and A5 files used in releases prior to release 40. 
      // Remove function this when we determine all sites are at release 40 or greater
      SD_A4A5_Rename();
      
      SD_Read_WiFiFile(); // Read wifi auth, ssid and pw

      // If elevation file exists it is opened, read and elevation set, else 0
      SD_ReadElevationFile();

     // If offset file exists it is opened, read and rain total rollover offset set, else 0
      SD_Read_RTRO_File();

      // Are we a Air Quality Station
      SD_CheckAQSFile();

      // See what has been define for these pins
      SD_CheckOP1Files();
      SD_CheckOP15MFile();  // only important if (scv.op1 == OP1_STATE_DISTANCE)
      SD_CheckOP2Files();

      if (!scv.aqs) {
        SD_CheckNoWindFile(); // if NOWIND.TXT found then scv.wind is set false
        SD_CheckNoRainFile(); // if NORAIN.TXT found then scv.rg1 is set false
      }
      SD_CheckOBITXIFiles();
    }

    // Do a check and make sure OBS and Transmit Times is at least 5m or greater when AQS is enabled
    if (scv.aqs) {
      if (scv.obi<5) {
        Output ("OBI Corrected 5M");
        scv.obi = 5;
      }
       if (scv.txi<5) {
        Output ("TXI Corrected 5M");
        scv.txi = 5;
      }  
    }
  }
}


/*
 * ======================================================================================================================
 * SD_CheckClearRainTotals()
 * If the SD card is available and CRT.TXT exists:
 *   1. Clear Rain Totals
 *   2. Delete CNV.TXT after successful config deletion
 *
 * Returns true only when CNV.TXT was found and all requested deletes succeeded.
 * ======================================================================================================================
 */
bool SD_CheckClearRainTotals() {
    // Do not access the SD filesystem unless it is known to be available.
    time32_t current_time = Time.now();
    Output("CRT:");
    EEPROM_ClearRainTotals(current_time);

    // Config files were successfully deleted; remove the trigger file too.
    if (unlink(SD_crt_file) == 0) {
        snprintf(msgbuf, sizeof(msgbuf), "CRT:%s Deleted", SD_crt_file);
        Output(msgbuf);
        return true;
    }

    snprintf(msgbuf, sizeof(msgbuf), "CRT:%s Delete Failed: %s", SD_crt_file, strerror(errno));
    Output(msgbuf);

    return false;
}