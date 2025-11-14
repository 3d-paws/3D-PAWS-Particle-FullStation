/*
 * ======================================================================================================================
 * ps.cpp - Particle Support Funtions
 * ======================================================================================================================
 */
#include <Particle.h>

#include "include/qc.h"
#include "include/cf.h"
#include "include/sdcard.h"
#include "include/eeprom.h"
#include "include/support.h"
#include "include/sensors.h"
#include "include/wrda.h"
#include "include/main.h"
#include "include/output.h"

 /*
 * ======================================================================================================================
 * GetPinName() - return the pin name in provider buffer
 * ======================================================================================================================
 */
void GetPinName(pin_t pin, char *pinname) {
  struct PinMap {
    pin_t pin;
    const char *name;
  };

#if (PLATFORM_ID == PLATFORM_MSOM)
  const PinMap pinTable[] = {
    { A0, "A0" },   // Same pin as D19 
    { A1, "A1" },   // Same pin as D18 
    { A2, "A2" },   // Same pin as D17 
    { A5, "A5" },   // Same pin as D14 
    { A6, "A6" },   // Same pin as D29
    { D0, "D0" },   // Same pin as SDA 
    { D1, "D1" },   // Same pin as SCL
    { D2, "D2" }, 
    { D3, "D3" },
    { D4, "D4" }, 
    { D5, "D5" }, 
    { D6, "D6" }, 
    { D9, "D9" },   // Same pin as TX
    { D10, "D10" }, // Same pin as RX
    { D11, "D11" }, // Same pin as MISO
    { D12, "D12" }, // Same pin as MOSI
    { D13, "D13" }, // Same pin as SCK
    { D20, "D20" }, { D21, "D21" }, { D22, "D22" }, { D24, "D24" },
    { D25, "D25" }, { D26, "D26" }, { D27, "D27" }
  };
#else
  const PinMap pinTable[] = {
    { A0, "A0" },   // Same pin as D19 
    { A1, "A1" },   // Same pin as D18 
    { A2, "A2" },   // Same pin as D17 
    { A3, "A3" },   // Same pin as D16
    { A4, "A4" },   // Same pin as D15
    { A5, "A5" },   // Same pin as D14 
    { D0, "D0" },   // Same pin as SDA 
    { D1, "D1" },   // Same pin as SCL
    { D2, "D2" }, 
    { D3, "D3" },
    { D4, "D4" }, 
    { D5, "D5" }, 
    { D6, "D6" }, 
    { D7, "D7" },
    { D8, "D8" },
    { D9, "D9" },   // Same pin as TX
    { D10, "D10" }, // Same pin as RX
    { D11, "D11" }, // Same pin as MISO
    { D12, "D12" }, // Same pin as MOSI
    { D13, "D13" }, // Same pin as SCK
  };
#endif

  for (size_t i = 0; i < sizeof(pinTable) / sizeof(pinTable[0]); ++i) {
      if (pin == pinTable[i].pin) {
          strcpy(pinname, pinTable[i].name);
          return;
      }
  }

  strcpy(pinname, "NF");
}

/*
 * ======================================================================================================================
 * OutputResetReason() - 
 * ======================================================================================================================
 */
void OutputResetReason() {
  struct ResetReasonEntry {
    int code;
    const char* description;
  };

  const ResetReasonEntry resetReasonTable[] = {
    {0,   "None / Unknown"},
    {10,  "Unknown Reset Reason"},
    {20,  "Reset Pin (button or pin)"},
    {30,  "Low Power Management Reset"},
    {40,  "Power Down Reset"},
    {50,  "Brownout Reset"},
    {60,  "Hardware Watchdog Reset"},
    {70,  "Successful Firmware Update"},
    {80,  "Firmware Update Error (deprecated)"},
    {90,  "Firmware Update Timeout"},
    {100, "Factory Reset Requested"},
    {110, "Safe Mode Requested"},
    {120, "DFU Mode Requested"},
    {130, "System Panic (SOS Code)"},
    {140, "User Reset (software call)"},
    // Add more entries here as needed
  };

  // Enable reset info feature to get valid reset reason data

  int resetReason = System.resetReason();
  uint32_t resetData = System.resetReasonData();

  // Find reset reason description by code inline
  const char* reasonText = "Unknown Reset Reason";
  for (unsigned int i = 0; i < sizeof(resetReasonTable) / sizeof(resetReasonTable[0]); i++) {
    if (resetReasonTable[i].code == resetReason) {
        reasonText = resetReasonTable[i].description;
        break;
    }
  }

  sprintf(msgbuf, "RR:%s (%d,%lu)", reasonText, resetReason, resetData);
  Output(msgbuf);
}

/*
 * ======================================================================================================================
 * Output_CellBatteryInfo() - On OLED display station information
 * ======================================================================================================================
 */
void Output_CellBatteryInfo() {
#if PLATFORM_ID == PLATFORM_ARGON
  WiFiSignal sig = WiFi.RSSI();
  float SignalStrength = sig.getStrength();

  sprintf (Buffer32Bytes, "CS:%d.%02d", 
    (int)SignalStrength, (int)(SignalStrength*100)%100);
  Output(Buffer32Bytes);
#else
  CellularSignal sig = Cellular.RSSI();
  float SignalStrength = sig.getStrength();
  
  int BatteryState = System.batteryState();
  float BatteryPoC = 0.0;                 // Battery Percent of Charge

  // Read battery charge information only if battery is connected. 
  if (BatteryState>0 && BatteryState<6) {
    BatteryPoC = System.batteryCharge();
  }
  
  sprintf (Buffer32Bytes, "CS:%d.%02d B:%d,%d.%02d", 
    (int)SignalStrength, (int)(SignalStrength*100)%100,
    BatteryState, (int)BatteryPoC, (int)(BatteryPoC*100)%100);
  Output(Buffer32Bytes);
#endif
}

/*
 * ======================================================================================================================
 * DeviceReset() - Kill power to ourselves and do a cold boot
 * ======================================================================================================================
 */
void DeviceReset() {
  digitalWrite(REBOOT_PIN, HIGH);
  delay(5000);
  // Should not get here if relay / watchdog is connected.
  digitalWrite(REBOOT_PIN, LOW);
  delay(2000); 
   // May never get here if relay cuts our power.
 
  // Resets the device, just like hitting the reset button or powering down and back up.
  System.reset();
}

/*
 * ======================================================================================================================
 * Function_DoAction() - Handler for Particle Function DoAction     
 * ======================================================================================================================
 */
int Function_DoAction(String s) {
  if (s.equals("REBOOT")) {  // Reboot - We loose untransmitted observations. But they are save to SD.
    Output("DoAction:REBOOT");     // Do a SEND before a REBOOT to address the abive issue.
    EEPROM_SaveUnreportedRain();
    delay(1000);

    DeviceReset();

    // Never gets here and thus can never send a ack to Particle Portal
    return(0);  
  }

  else if (s.equals("INFO")) {  // Send System Information
    Output("DoAction:INFO");
    SendSystemInformation=true;
    return(0);  
  }

  else if (s.equals("SEND")) {  // Send OBS Now
    Output("DoAction:SEND");
    LastTransmitTime=0;
    return(0);  
  }

  else if (s.equals("CRT")) { // Clear Rain Totals
    time32_t current_time = Time.now();
    Output("DoAction:CRT");
    EEPROM_ClearRainTotals(current_time);
    // Display EEPROM Information 
    // EEPROM_Dump();   
    return(0);
  }

  else if (s.equals("OP1DIST")) { // Set OP1 State File to Distance
    Output("DoAction:OP1DIST");
    if (SD_exists) {
      if (SD.exists(SD_OP1_RAIN_FILE)) {
        EEPROM_ClearRain2Totals();
        if (SD.remove (SD_OP1_RAIN_FILE)) {
          Output ("OP1=DIST, DEL RAIN:OK");
        }
        else {
          Output ("OP1=DIST, DEL RAIN:ERR");
          return(-2);
        }
      }

      if (SD.exists(SD_OP1_DIST_FILE)) {
        Output ("OP1=DIST, ALREADY EXISTS");    
      }
      else {
        // Touch File
        File fp = SD.open(SD_OP1_DIST_FILE, FILE_WRITE);
        if (fp) {
          fp.close();
          Output ("OP1=DIST, SET");
        }
        else {
          Output ("OP1=DIST, OPEN ERR");
          return(-3);
        }
      }

      // Set Distance Sensor Divisor to that of 10m
      dg_adjustment = 2.5;
      if (SD.exists(SD_OP1_D5M_FILE)) {
        if (SD.remove (SD_OP1_D5M_FILE)) {
          Output ("OP1=DIST, DEL 5M:OK, 10M SET");
        }
        else {
          Output ("OP1=DIST, DEL 5M:ERR");
          return(-4);
        }
      }
      else {
        Output ("OP1=DIST, 10M");
      }
    }
    else {
      Output("OP1=DIST, SD NF"); 
      return(-1);      
    }
 
    return(0);
  }

  else if (s.equals("OP1RAIN")) { // Set OP1 State File to Rain
    Output("DoAction:OP1RAIN");
    if (SD_exists) {
      if (SD.exists(SD_OP1_DIST_FILE)) {
        if (SD.remove (SD_OP1_DIST_FILE)) {
          Output ("OP1=RAIN, DEL DIST:OK");
        }
        else {
          Output ("OP1=RAIN, DEL DIST:ERR");
          return(-2);
        }
      }
      if (SD.exists(SD_OP1_D5M_FILE)) {
        if (SD.remove (SD_OP1_D5M_FILE)) {
          Output ("OP1=RAIN, DEL 5M:OK");
        }
        else {
          Output ("OP1=RAIN, DEL 5M:ERR");
          return(-4);
        }
      }

      if (SD.exists(SD_OP1_RAIN_FILE)) {
        Output ("OP1=RAIN, ALREADY EXISTS");      
      }
      else {
        EEPROM_ClearRain2Totals(); // Just a good thing to do.
        // Touch File
        File fp = SD.open(SD_OP1_RAIN_FILE, FILE_WRITE);
        if (fp) {
          fp.close();
          Output ("OP1=RAIN, SET");
        }
        else {
          Output ("OP1=RAIN, OPEN ERR");
          return(-3);
        }
      }
    }
    else {
      Output("OP1=RAIN, SD NF"); 
      return(-1);      
    }
    return(0);
  }

  else if (s.equals("OP1D5M")) { // Set 5M Distance Sensor State File
    Output("DoAction:OP1D5M");
    if (SD_exists) {
      if (SD.exists(SD_OP1_D5M_FILE)) {
        Output ("OP1D5M, ALREADY EXISTS");      
      }
      else {
        // Touch File
        File fp = SD.open(SD_OP1_D5M_FILE, FILE_WRITE);
        if (fp) {
          fp.close();
          dg_adjustment = 1.25;
          Output ("OP1D5M SET");
        }
        else {
          Output ("OP1D5M OPEN ERR");
          return(-5);
        }
      }
    }
    else {
      Output("OP1D5M, SD NF"); 
      return(-1);      
    }
    return(0);
  }

  else if (s.equals("OP1RAW")) { // Set OP1 State File to Raw
    Output("DoAction:OP1RAW");
    if (SD_exists) {

      // Remove Rain Configuration
      if (SD.exists(SD_OP1_RAIN_FILE)) {
        EEPROM_ClearRain2Totals();
        if (SD.remove (SD_OP1_RAIN_FILE)) {
          Output ("OP1=DIST, DEL RAIN:OK");
        }
        else {
          Output ("OP1=DIST, DEL RAIN:ERR");
          return(-2);
        }
      }

      // Remove Distance Configuration
      if (SD.exists(SD_OP1_DIST_FILE)) {
        if (SD.remove (SD_OP1_DIST_FILE)) {
          Output ("OP1=DIST, DEL DIST:OK");
        }
        else {
          Output ("OP1=DIST, DEL DIST:ERR");
          return(-3);
        }
      }

      // Remove Distanve sensor type and reset distance adjustment to default 10m
      dg_adjustment = 2.5;
      if (SD.exists(SD_OP1_D5M_FILE)) {
        if (SD.remove (SD_OP1_D5M_FILE)) {
          Output ("OP1=DIST, DEL 5M:OK");
        }
        else {
          Output ("OP1=DIST, DEL 5M:ERR");
          return(-4);
        }
      }

      // Add OP1 Raw configuration
      if (SD.exists(SD_OP1_RAW_FILE)) {
        Output ("OP1=RAW, ALREADY EXISTS");    
      }
      else {
        // Touch File
        File fp = SD.open(SD_OP1_RAW_FILE, FILE_WRITE);
        if (fp) {
          fp.close();
          Output ("OP1=RAW, SET");
        }
        else {
          Output ("OP1=RAW, OPEN ERR");
          return(-5);
        }
      }
    }
    else {
      Output("OP1=RAW, SD NF"); 
      return(-1);      
    }
    return(0);
  }

  else if (s.equals("OP1CLR")) { // Clear OP1 State Files
    int state=0;
    Output("DoAction:OP1CLR");
    if (SD_exists) {
      if (SD.exists(SD_OP1_DIST_FILE)) {
        if (SD.remove (SD_OP1_DIST_FILE)) {
          OP1_State = OP1_STATE_NULL;
          Output ("OP1=CLR, DEL DIST:OK");
        }
        else {
          Output ("OP1=CLR, DEL DIST:ERR");
          state=-2;
        }
      }
      else {
        Output ("OP1=CLR, DEL RAIN:NF");
      }

      if (SD.exists(SD_OP1_RAIN_FILE)) {
        if (SD.remove (SD_OP1_RAIN_FILE)) {
          OP1_State = OP1_STATE_NULL;      // We still need a reboot to get rid of ISR
          Output ("OP1=CLR, DEL RAIN:OK");
        }
        else {
          Output ("OP1=CLR, DEL RAIN:ERR");
          state+=-3; // returns a -3 if also failed removing DIST file
        }
      }
      else {
        Output ("OP1=CLR, DEL RAIN:NF");
      }

      if (SD.exists(SD_OP1_RAW_FILE)) {
        if (SD.remove (SD_OP1_RAW_FILE)) {
          OP1_State = OP1_STATE_NULL;
          Output ("OP1=CLR, DEL RAW:OK");
        }
        else {
          Output ("OP1=CLR, DEL RAW:ERR");
          state+=-4; // returns a -4 if also failed removing RAW file
        }
      }
      else {
        Output ("OP1=CLR, DEL RAIN:NF");
      }

      if (SD.exists(SD_OP1_D5M_FILE)) {
        if (SD.remove (SD_OP1_D5M_FILE)) {
          Output ("OP1=CLR, DEL 5M:OK");
          dg_adjustment = 2.5;
        }
        else {
          Output ("OP1=CLR, DEL 5M:ERR");
          state+=-5;
        }
      }
    }
    else {
      Output("OP1=CLR, SD NF"); 
      state=-1;     
    }
    return(state);
  }

  else if (s.equals("OP2RAW")) { // Set OP2 State File to Raw
    Output("DoAction:OP2RAW");
    // Add OP2 Raw configuration
    if (SD_exists) {
      if (SD.exists(SD_OP2_RAW_FILE)) {
        Output ("OP2=RAW, ALREADY EXISTS");    
      }
      else {
        // Touch File
        File fp = SD.open(SD_OP2_RAW_FILE, FILE_WRITE);
        if (fp) {
          fp.close();
          OP2_State = OP2_STATE_RAW;
          Output ("OP2=RAW, SET");
        }
        else {
          Output ("OP2=RAW, OPEN ERR");
          return(-2);
        }
      }
    }
    else {
      Output("OP2=RAW, SD NF"); 
      return(-1);      
    }
    return(0);
  }

    
  else if (s.equals("OP2CLR")) { // Clear OP2 State Files
    int state=0;
    Output("DoAction:OP2CLR");
    if (SD_exists) {
      if (SD.exists(SD_OP2_RAW_FILE)) {
        if (SD.remove (SD_OP2_RAW_FILE)) {
          OP2_State = OP2_STATE_NULL;
          Output ("OP2=CLR, DEL RAW:OK");
        }
        else {
          Output ("OP2=CLR, DEL RAW:ERR");
          state=-2;
        }
      }
      else {
        Output ("OP2=CLR, DEL OP2RAW:NF");
      }
    }
    else {
      Output("OP2=CLR, SD NF"); 
      state=-1;     
    }
    return(state);
  }

  else if (s.equals("TXI5M")) { // Set 1 Minute Observations, Transmit Interval to 5 Minutes
    Output("DoAction:TXI5M");
    sprintf (msgbuf, "CUR: OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
    Output(msgbuf);  

    if (SD_exists) {
      if (SD.exists(SD_TX5M_FILE)) {
        Output ("TXI5M, ALREADY SET");     
      }
      else {
        // Touch File
        File fp = SD.open(SD_TX5M_FILE, FILE_WRITE);
        if (fp) {
          fp.close();
          //SD_RemoveFile (SD_TX5M_FILE);
          SD_RemoveFile (SD_TX10M_FILE);
          SD_RemoveFile (SD_OB5M_FILE);
          SD_RemoveFile (SD_OB10M_FILE);
          SD_RemoveFile (SD_OB15M_FILE);
          obs_interval = DEFAULT_OBS_INTERVAL;
          obs_tx_interval = 5;
          Output ("TXI5M SET");
        }
        else {
          Output ("TXI5M OPEN ERR");
          return(-2);
        }
        sprintf (msgbuf, "NEW: OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
        Output(msgbuf);
      }
    }
    else {
      Output("TXI5M, SD NF"); 
      return(-1);      
    }
    return(0);
  }

  else if (s.equals("TXI10M")) { // Set 1 Minute Observations, Transmit Interval to 10 Minutes
    Output("DoAction:TXI10M");
    sprintf (msgbuf, "CUR: OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
    Output(msgbuf);  

    if (SD_exists) {
      if (SD.exists(SD_TX10M_FILE)) {
        Output ("TXI10M, ALREADY SET");  
      }
      else {
        // Touch File
        File fp = SD.open(SD_TX10M_FILE, FILE_WRITE);
        if (fp) {
          fp.close();
          Output ("TXI10M SET");
          SD_RemoveFile (SD_TX5M_FILE);
          //SD_RemoveFile (SD_TX10M_FILE);
          SD_RemoveFile (SD_OB5M_FILE);
          SD_RemoveFile (SD_OB10M_FILE);
          SD_RemoveFile (SD_OB15M_FILE);
          obs_interval = DEFAULT_OBS_INTERVAL;
          obs_tx_interval = 10;
        }
        else {
          Output ("TXI10M OPEN ERR");
          return(-2);
        }
        sprintf (msgbuf, "SET: OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
        Output(msgbuf);
      }
    }
    else {
      Output("TXI10M, SD NF"); 
      return(-1);      
    }
    return(0);
  }

  else if (s.equals("TXI15M")) { // Set 1 Minute Observations, Transmit Interval to 15 Minutes, 
    Output("DoAction:TXI15M");
    sprintf (msgbuf, "CUR: OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
    Output(msgbuf);

    if (SD_exists) {
      SD_RemoveFile (SD_TX5M_FILE);
      SD_RemoveFile (SD_TX10M_FILE);
      SD_RemoveFile (SD_OB5M_FILE);
      SD_RemoveFile (SD_OB10M_FILE);
      SD_RemoveFile (SD_OB15M_FILE);
      obs_interval = DEFAULT_OBS_INTERVAL;
      obs_tx_interval = DEFAULT_OBS_TRANSMIT_INTERVAL;
      sprintf (msgbuf, "SET: OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
      Output(msgbuf);
    }
    else {
      Output("TXI15M, SD NF"); 
      return(-1);      
    }
    return(0);
  }

  else if (s.equals("OBI5M")) { // Set 5 Minute Observations, Transmit Interval to 5 Minutes
    Output("DoAction:OBI5M");
    sprintf (msgbuf, "CUR: OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
    Output(msgbuf);  

    if (SD_exists) {
      if (SD.exists(SD_OB5M_FILE)) {
        Output ("OBI5M, ALREADY SET");  
      }
      else {
        // Touch File
        File fp = SD.open(SD_OB5M_FILE, FILE_WRITE);
        if (fp) {
          fp.close();
          Output ("OBI5M SET");
          SD_RemoveFile (SD_TX5M_FILE);
          SD_RemoveFile (SD_TX10M_FILE);
          // SD_RemoveFile (SD_OB5M_FILE);
          SD_RemoveFile (SD_OB10M_FILE);
          SD_RemoveFile (SD_OB15M_FILE);
          obs_interval = obs_tx_interval = 5;
        }
        else {
          Output ("OBI5M OPEN ERR");
          return(-2);
        }
        sprintf (msgbuf, "SET: OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
        Output(msgbuf);
      }
    }
    else {
      Output("OBI5M, SD NF"); 
      return(-1);      
    }
    return(0);
  }

  else if (s.equals("OBI10M")) { // Set 10 Minute Observations, Transmit Interval to 10 Minutes
    Output("DoAction:OBI10M");
    sprintf (msgbuf, "CUR: OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
    Output(msgbuf);  

    if (SD_exists) {
      if (SD.exists(SD_OB10M_FILE)) {
        Output ("OBI10M, ALREADY SET");  
      }
      else {
        // Touch File
        File fp = SD.open(SD_OB10M_FILE, FILE_WRITE);
        if (fp) {
          fp.close();
          Output ("OBI10M SET");
          SD_RemoveFile (SD_TX5M_FILE);
          SD_RemoveFile (SD_TX10M_FILE);
          SD_RemoveFile (SD_OB5M_FILE);
          // SD_RemoveFile (SD_OB10M_FILE);
          SD_RemoveFile (SD_OB15M_FILE);
          obs_interval = obs_tx_interval = 10;
        }
        else {
          Output ("OBI10M OPEN ERR");
          return(-2);
        }
        sprintf (msgbuf, "SET: OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
        Output(msgbuf);
      }
    }
    else {
      Output("OBI10M, SD NF"); 
      return(-1);      
    }
    return(0);
  }

  else if (s.equals("OBI105")) { // Set 15 Minute Observations, Transmit Interval to 15 Minutes
    Output("DoAction:OBI15M");
    sprintf (msgbuf, "CUR: OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
    Output(msgbuf);  

    if (SD_exists) {
      if (SD.exists(SD_OB15M_FILE)) {
        Output ("OBI15M, ALREADY SET");  
      }
      else {
        // Touch File
        File fp = SD.open(SD_OB15M_FILE, FILE_WRITE);
        if (fp) {
          fp.close();
          Output ("OBI15M SET");
          SD_RemoveFile (SD_TX5M_FILE);
          SD_RemoveFile (SD_TX10M_FILE);
          SD_RemoveFile (SD_OB5M_FILE);
          SD_RemoveFile (SD_OB10M_FILE);
          //SD_RemoveFile (SD_OB15M_FILE);
          obs_interval = obs_tx_interval = 15;
        }
        else {
          Output ("OBI15M OPEN ERR");
          return(-2);
        }
        sprintf (msgbuf, "SET: OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
        Output(msgbuf);
      }
    }
    else {
      Output("OBI15M, SD NF"); 
      return(-1);      
    }
    return(0);
  }

  else if (s.equals("OPTAQS")) { // Enable Air Quality Station
    Output("DoAction:OPTAQS");
    if (SD_exists) {
      if (SD.exists(SD_OPTAQS_FILE)) {
        Output ("OPTAQS, ALREADY SET");  
      }
      else {
        // Touch File
        File fp = SD.open(SD_OPTAQS_FILE, FILE_WRITE);
        if (fp) {
          fp.close();
          Output ("OPTAQS SET");
        }
        else {
          Output ("OPTAQS OPEN ERR");
          return(-2);
        }
      }
    }
    else {
      Output("OPTAQS, SD NF"); 
      return(-1);      
    }
    return(0);
  }

  else if (s.equals("OPTFS")) { // Enable Air Quality Station
    Output("DoAction:OPTFS");
    if (SD_exists) {
      if (SD.exists(SD_OPTAQS_FILE)) {
        SD_RemoveFile (SD_OPTAQS_FILE);

        // Switching to Full Station Clear Rain Totals from EEPROM
        time32_t current_time = Time.now();
        EEPROM_ClearRainTotals(current_time);

        Output ("OPTFS SET");
      }
      else {
        Output ("OPTFS, ALREADY SET");  
      }
    }
    else {
      Output("OPTAQS, SD NF"); 
      return(-1);      
    }
    return(0);
  }

  else if (s.indexOf("SETELEV:" == 0)) { // Pattern start of string aka 0 offset
    Output("DoAction:SETELEV");
    String rest = s.substring(8);   // get part after "SETELEV:", 8 = length of 
    long elevation = rest.toInt();  // convert to integer
    if ((String(elevation) == rest) && (elevation >= QC_MIN_ELEV) && (elevation <= QC_MAX_ELEV)) {
      if (SD_exists) {
        if (SD.exists(SD_ELEV_FILE)) { 
          SD_RemoveFile (SD_ELEV_FILE);
        }
        File file = SD.open(SD_ELEV_FILE, FILE_WRITE);
        if (file) {
          file.print(elevation);  // write the elevation to the file
          file.close();           // save and close the file
          sprintf (Buffer32Bytes, "SETELEV:%ld OK", elevation);

          cf_elevation = elevation; // Set running value of elevation
        } 
        else {
          sprintf (Buffer32Bytes, "SETELEV:%ld FAIL", elevation); 
        }
        Output (Buffer32Bytes);
      }
      else {
        Output("SETELEV, SD NF"); 
        return(-1);      
      }
    }
    else {
      Output("SETELEV, INVALID ELEV#"); 
      return(-2);           
    }
    return(0);
  }

  else {
    Output("DoAction:UKN"); 
    return(-1);
  }
}

#if PLATFORM_ID == PLATFORM_ARGON
/*
 * ======================================================================================================================
 * WiFiChangeCheck() - Check for WIFI.TXT file and set Wireless SSID, Password            
 * ======================================================================================================================
 */
void WiFiChangeCheck() {
  File fp;
  int i=0;
  char *p, *auth, *ssid, *pw;
  char ch, buf[128];
  bool changed = false;

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
          while (fp.available() && (i < 127 )) {
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
          buf[i] = (char) NULL;

          // Parse string for the following
          //   WIFI ssid password
          p = &buf[0];
          auth = strtok_r(p, ",", &p);

          if (auth == NULL) {
            Output("WIFI:ID=Null Err");
          }
          else if ( (strcmp (auth, "WEP") != 0)  &&
                    (strcmp (auth, "WPA") != 0)  &&
                    (strcmp (auth, "WPA2") != 0) &&
                    (strcmp (auth, "UNSEC") != 0)) {
            sprintf (msgbuf, "WIFI:ATYPE[%s] Err", auth);          
            Output(msgbuf);
          }
          else {
            ssid = strtok_r(p, ",", &p);
            pw  = strtok_r(p, ",", &p);
            
            if (pw == NULL) {
              pw = (char *) "";  // Handle the case when nothing is after the ","
            }

            if (ssid == NULL) {
              Output("WIFI:SSID=Null Err");
            }

            // UNSEC is allowed to have no password just a ssid, but non of the others
            else if ((strcmp (auth, "UNSEC") != 0)  && (pw == NULL)) {
              Output("WIFI:PW=Null Err");
            }
            else {
              sprintf (msgbuf, "WIFI:ATYPE[%s]", auth);          
              Output(msgbuf);
              sprintf (msgbuf, "WIFI:SSID[%s]", ssid);
              Output(msgbuf);
              sprintf (msgbuf, "WIFI:PW[%s]", pw);
              Output(msgbuf);

              // Connects to a network secured with WPA2 credentials.
              // https://docs.particle.io/reference/device-os/api/wifi/securitytype-enum/
              if (strcmp (auth, "UNSEC") == 0) {
                Output("WIFI:Credentials Cleared");
                WiFi.clearCredentials();
                if (strcmp (pw, "") == 0) {
                  Output("WIFI:Credentials Set UNSEC NO PW");
                  WiFi.setCredentials(ssid);
                }
                else {
                  Output("WIFI:Credentials Set UNSEC");
                  WiFi.setCredentials(ssid, pw);                 
                }
              }
              else if (strcmp (auth, "WEP") == 0) {
                Output("WIFI:Credentials Cleared");
                WiFi.clearCredentials();
                Output("WIFI:Credentials Set WEP");
                WiFi.setCredentials(ssid, pw, WEP);
              }
              else if (strcmp (auth, "WPA") == 0) {
                Output("WIFI:Credentials Cleared");
                WiFi.clearCredentials();
                Output("WIFI:Credentials Set WPA");
                WiFi.setCredentials(ssid, pw, WPA);
              }
              else if (strcmp (auth, "WPA2") == 0) {
                Output("WIFI:Credentials Cleared");
                WiFi.clearCredentials();
                Output("WIFI:Credentials Set WPA2");
                WiFi.setCredentials(ssid, pw, WPA2);
              }
              else if (strcmp (auth, "WPA_ENTERPRISE") == 0) {
                // WPA Enterprise is only supported on the Photon and P1.
                // It is not supported on the Argon, P2, and Photon 2.
                Output("WIFI:Credentials Cleared");
                WiFi.clearCredentials();
                Output("WIFI:Credentials Set WPAE");
                WiFi.setCredentials(ssid, pw, WPA_ENTERPRISE);
              }
              else if (strcmp (auth, "WPA2_ENTERPRISE") == 0) {
                // WPA Enterprise is only supported on the Photon and P1.
                // It is not supported on the Argon, P2, and Photon 2.
                Output("WIFI:Credentials Cleared");
                WiFi.clearCredentials();
                Output("WIFI:Credentials Set WPAE2");
                WiFi.setCredentials(ssid, pw, WPA2_ENTERPRISE);
              }
              else { 
                Output("WIFI:Credentials NOT Set");
                Output("WIFI:USING NVAUTH");
              }
            }
          }
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
#endif

#if PLATFORM_ID == PLATFORM_MSOM
/*
 * ======================================================================================================================
 * network_initialize() - Setup WiFi if WIFI.TXT exists. Else setup Cellular            
 * ======================================================================================================================
 */
void network_initialize() {
  File fp;
  int i=0;
  char *p, *id, *ssid, *pw;
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
          while (fp.available() && (i < 127 )) {
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
          buf[i] = (char) NULL;

          // Parse string for the following
          //   WIFI ssid password
          p = &buf[0];
          id = strtok_r(p, ",", &p);

          if (id == NULL) {
            Output("WIFI:ID=Null Err");
          }
          else if (strcmp (id, "MUON") != 0) { 
            sprintf (msgbuf, "WIFI:ID[%s] Err", id);          
            Output(msgbuf);
          }
          else {
            ssid = strtok_r(p, ",", &p);
            pw  = strtok_r(p, ",", &p);
            
            if (ssid == NULL) {
              Output("WIFI:SSID=Null Err");
            }
            else if (pw == NULL) {
              Output("WIFI:PW=Null Err");
            }
            else {
              Output("NETWORK:SET WIFI");
              MuonWifiEnabled = true;

              sprintf (msgbuf, "WIFI:SSID[%s]", ssid);
              Output(msgbuf);
              sprintf (msgbuf, "WIFI:PW[%s]", pw);
              Output(msgbuf);

              Output("WIFI:Particle Cloud Disconnect");  // We should no be connected, but do anyway
              Particle.disconnect();

              Output("WIFI:Turning Off Cellular");
              Cellular.off();    // Turn off cellular modem
              waitUntil(Cellular.isOff);  // Optional: wait for cellular modem to power down

              Output("WIFI:Turning On Wifi");
              WiFi.on();
              
              if (WiFi.clearCredentials()) {
                Output("WIFI:Cleared Wifi Creds");
              } else {
                Output("WIFI:Clear Wifi Creds Err");
              }

              if (WiFi.setCredentials(ssid, pw)) {
                Output("WIFI:Credentials Set");
              } else {
                Output("WIFI:Credentials Set Err");
              }

              Output("WIFI:Connect Called");
              WiFi.connect();
              // waitUntil(WiFi.ready);  // No we want to move on with out network
            }
          }
        }
      }
      else {
        sprintf (msgbuf, "WIFI:OPENERR[%s]", SD_wifi_file);          
        Output(msgbuf);
        Output ("WIFI:USING CELLULAR");
      }
    } 
    else {
      Output ("WIFO:NOFILE USING CELLULAR");
    }
  } // SD enabled
  else {
    Output ("WIFI:NOSD USING CELLULAR");
  }

  if (MuonWifiEnabled == false) {
    Output("NETWORK:SET CELL");
    Output("CELL:Particle Cloud Disconnect");  // We should no be connected, but do anyway
    Particle.disconnect();

    Output("CELL:Turning Off WiFi");
    WiFi.disconnect();      // Disconnect Wi-Fi cleanly
    WiFi.off();             // Turn Wi-Fi radio off to save power and avoid interference

    if (WiFi.clearCredentials()) {
      Output("CELL:Cleared Wifi Creds");
    } else {
      Output("CELL:Clear Wifi Creds Err");
    }

    Output("CELL:Turning On Cellular");
    Cellular.on();          // Power on cellular modem
  }
}
#endif

#if (PLATFORM_ID == PLATFORM_ARGON) || (PLATFORM_ID == PLATFORM_MSOM)
/*
 * ======================================================================================================================
 * WiFiPrintCredentials() - Read NVRAM and print WiFi Creditials     
 * 
 * Particle's Device OS does not provide a direct method to determine which of the stored Wi-Fi credentials is currently 
 * being used for the connection. However, you can infer this indirectly by attempting to connect to each stored Wi-Fi 
 * network and checking if the connection is successful.        
 * ======================================================================================================================
 */
void WiFiPrintCredentials() {
  byte mac[6];

  WiFi.macAddress(mac);

  sprintf (msgbuf, "WIFI MAC[%02x:%02x:%02x:%02x:%02x:%02x]", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Output(msgbuf);

  WiFiAccessPoint ap[5];
  WiFi.getCredentials(ap, 5);
  int found = 0;

  for (int i = 0; i < 5; i++) {
    if (strlen(ap[i].ssid) != 0) {
      found++;
      if (found == 1) {
        Output("WIFI Creds:");
      }
      sprintf (msgbuf, "IDX[%d]", i);
      Output(msgbuf);
      sprintf (msgbuf, " SSID[%s]", ap[i].ssid);
      Output(msgbuf);
      switch (ap[i].security) {
        case WLAN_SEC_UNSEC:
          Output(" Type:Unsecured");
          break;
        case WLAN_SEC_WEP:
          Output(" Type:WEP");
          break;
        case WLAN_SEC_WPA:
          Output(" Type:WPA");
          break;
        case WLAN_SEC_WPA2:
          Output(" Type:WPA2");
          break;
        case WLAN_SEC_WPA_ENTERPRISE:
          Output(" Type:WPA Ent");
          break;
        case WLAN_SEC_WPA2_ENTERPRISE:
          Output(" Type:WPA2 Ent");
          break;
        default:
          Output(" Type:Unknown");
          break;
      }
      sprintf (msgbuf, " SigStr[%d]", ap[i].rssi);
      Output(msgbuf);
      sprintf (msgbuf, " Channel[%d]", ap[i].channel);
      Output(msgbuf);
      sprintf (msgbuf, " BMAC[%02x:%02x:%02x:%02x:%02x:%02x]", 
        ap[i].bssid[0], ap[i].bssid[1], ap[i].bssid[2], ap[i].bssid[3], ap[i].bssid[4], ap[i].bssid[5]);
      Output(msgbuf);
    }
  }
  if (!found) {
    Output("WIFI Creds:NF");
  }
}
#else
/*
 * ======================================================================================================================
 * SimChangeCheck() - Check for SIM.TXT file and set sim based on contents, after rename file to SIMOLD.TXT            
 * ======================================================================================================================
 */
void SimChangeCheck() {
  File fp;
  int i=0;
  char *p, *id, *apn, *un, *pw;
  char ch, buf[128];
  bool changed = false;

  SimType simType = Cellular.getActiveSim();

  if (simType == 1) {
    Output ("SIM:Internal");
  } else if (simType == 2) {
    Output ("SIM:External");
  }
  else {
    sprintf (msgbuf, "SIM:Unknown[%d]", simType);
    Output (msgbuf);
  }

  if (SD_exists) {
    // Test for file SIM.TXT
    if (SD.exists(SD_sim_file)) {
      fp = SD.open(SD_sim_file, FILE_READ); // Open the file for reading, starting at the beginning of the file.

      if (fp) {
        // Deal with too small or too big of file
        if (fp.size()<=7 || fp.size()>127) {
          fp.close();
          Output ("SIMF:Invalid SZ");
          if (SD.remove (SD_sim_file)) {
            Output ("SIMF->Del:OK");
          }
          else {
            Output ("SIMF->Del:Err");
          }
        }
        else {
          Output ("SIMF:Open");
          while (fp.available() && (i < 127 )) {
            ch = fp.read();

            if ((ch == 0x0A) || (ch == 0x0D) ) {  // newline or linefeed
              break;
            }
            else {
              buf[i++] = ch;
            }
          }

          // At this point we have encountered EOF, CR, or LF
          // Now we need to terminate array with a null to make it a string
          buf[i] = (char) NULL;

          // Parse string for the following
          //   INTERNAL
          //   AUP epc.tmobile.com username passwd
          //   UP username password
          //   APN epc.tmobile.com
          p = &buf[0];
          id = strtok_r(p, " ", &p);

          if (id != NULL) {
            sprintf (msgbuf, "SIMF:ID[%s]", id);
            Output(msgbuf);
          }

          if (strcmp (id,"INTERNAL") == 0) {
            Cellular.setActiveSim(INTERNAL_SIM);
            Cellular.clearCredentials();
            changed = true;
          }
          else if (strcmp (id,"APN") == 0) {
            apn = strtok_r(p, " ", &p);

            if (apn == NULL) {
              Output("SIMF:APN=Null Err");
            }
            else {
              Cellular.setActiveSim(EXTERNAL_SIM);
              Output("SIM:Set External-APN");

              // Connects to a cellular network by APN only
              Cellular.setCredentials(apn);
              Output("SIM:Set Credentials");
              sprintf (msgbuf, " APN[%s]", apn);
              Output(msgbuf);
              changed = true;
            }
          }
          else if (strcmp (id," UP") == 0) {
            un  = strtok_r(p, " ", &p);
            pw  = strtok_r(p, " ", &p);

            if (un == NULL) {
              Output("SIMF:Username=Null Err");
            }
            else if (pw == NULL) {
              Output("SIMF:Passwd=Null Err");
            }
            else {
              Cellular.setActiveSim(EXTERNAL_SIM);
              Output("SIM:Set External-UP");

              // Connects to a cellular network with USERNAME and PASSWORD only
              Cellular.setCredentials(un,pw);
              Output("SIM:Set Credentials");
              sprintf (msgbuf, " UN[%s]", un);
              Output(msgbuf);
              sprintf (msgbuf, " PW[%s]", pw);
              Output(msgbuf);
              changed = true;
            }
          }
          else if (strcmp (id,"AUP") == 0) {
            apn = strtok_r(p, " ", &p);
            un  = strtok_r(p, " ", &p);
            pw  = strtok_r(p, " ", &p);

            if (apn == NULL) {
              Output("SIMF:APN=Null Err");
            }
            else if (un == NULL) {
              Output("SIMF:Username=Null Err");
            }
            else if (pw == NULL) {
              Output("SIMF:Passwd=Null Err");
            }
            else {
              Cellular.setActiveSim(EXTERNAL_SIM);
              Output("SIM:Set External-AUP");

              // Connects to a cellular network with a specified APN, USERNAME and PASSWORD
              Cellular.setCredentials(apn,un,pw);
              Output("SIM:Set Credentials");
              sprintf (msgbuf, " APN[%s]", apn);
              Output(msgbuf);
              sprintf (msgbuf, "  UN[%s]", un);
              Output(msgbuf);
              sprintf (msgbuf, "  PW[%s]", pw);
              Output(msgbuf);
              changed = true;
            }
          }
          else {  // Pasrse Error
            sprintf (msgbuf, "SIMF:ID[%s] Err", id);
            Output(msgbuf);
          }
        }

        // No matter what happened with the above, rename file so we don't process again at boot
        // if SIMOLD.TXT exists then remove it before we rename SIM.TXT
        if (SD.exists(SD_simold_file)) {
          if (SD.remove (SD_simold_file)) {
            Output ("SIMF:DEL SIMOLD");
          }
        }

        if (!fp.rename(SD_simold_file)) {
          Output ("SIMF:RENAME ERROR");
        }
        else {
          Output ("SIMF:RENAME OK");
        }
        fp.close();

        // Notify user to reboot blink led forever
        if (changed) {
          Output ("==============");
          Output ("!!! REBOOT !!!");
          Output ("==============");

        }
        else {
          Output ("=====================");
          Output ("!!! SET SIM ERROR !!!");
          Output ("=====================");
        }
        
        while(true) { // wait for Host to open serial port
          Blink(1, 750);
        }
      }
      else {
        Output ("SIMF:OPEN ERROR");
      }
    } 
    else {
      Output ("SIM:NO UPDATE FILE");
    }
  } // Console and SD enabled
}
#endif

#if (PLATFORM_ID == PLATFORM_BORON) || (PLATFORM_ID == PLATFORM_MSOM)

char imsi[16] = "";  // International Mobile Subscriber Identity

/*
 * ======================================================================================================================
 * callback_imsi() - Callback for International Mobile Subscriber Identity 
 * 
 * COMMAND: AT+CIMI   Note: Each line returned is a call to this callback function
 * SEE https://docs.particle.io/reference/device-os/api/cellular/command/
 * ======================================================================================================================
 */
int callback_imsi(int type, const char* buf, int len, char* cimi) {
  // sprintf (msgbuf, "AT+CIMI:%X [%s]", type, buf); Output (msgbuf);     

  if ((type == TYPE_UNKNOWN) && cimi) {
    if (sscanf(buf, "\r\n%[^\r]\r\n", cimi) == 1)
      /*nothing*/;
  }

  if (type == TYPE_OK) {
    return (RESP_OK);
  }
  return (WAIT);
}
#endif

/* 
 *=======================================================================================================================
 * OBI_AQS_Initialize() - Check SD Card for file to determine if we are a Air Quality Station
 *=======================================================================================================================
 */
void OBI_AQS_Initialize() {
  Output ("OBSAQS:INIT");
  if (SD_exists) {
    if (SD.exists(SD_OPTAQS_FILE)) {
      Output ("OPTAQI Enabled");

      // Ware are a Air Quality Station so Clear Rain Totals from EEPROM
      time32_t current_time = Time.now();
      EEPROM_ClearRainTotals(current_time);

      pinMode (OP2_PIN, OUTPUT);
      digitalWrite(OP2_PIN, HIGH); // Turn on Air Quality Sensor

      // We will only go in AQS mode if the sensor is truely there
      AQS_Enabled = true;
      AQS_Correction = (AQSWarmUpTime + 10) * 1000;  // In ms. Correction to be subtracted from mainloop poll interval 
                                                     // to account for the AQS warmup time and 10s for sampling
    }
    else {
      Output ("OPTAQI NF");
      AQS_Enabled = false;
    }
  }
}

/* 
 *=======================================================================================================================
 * OBI_TXI_Initialize() - Observation Interval Transmit Interval Initialize
 *=======================================================================================================================
 */
void OBI_TXI_Initialize() {
  Output ("OBSTXI:INIT");
  if (SD_exists) {
    if (SD.exists(SD_TX5M_FILE)) {
      Output ("TXI5M Found");
      obs_tx_interval = 5;
      SD_RemoveFile (SD_TX10M_FILE);
      SD_RemoveFile (SD_OB5M_FILE);
      SD_RemoveFile (SD_OB10M_FILE);
      SD_RemoveFile (SD_OB15M_FILE);
    }
    else if (SD.exists(SD_TX10M_FILE)) {
      Output ("TXI10M Found");
      obs_tx_interval = 10;
      SD_RemoveFile (SD_OB5M_FILE);
      SD_RemoveFile (SD_OB10M_FILE);
      SD_RemoveFile (SD_OB15M_FILE);
    }
    else if (SD.exists(SD_OB5M_FILE)) {
      Output ("OBI5M Found");
      obs_interval = obs_tx_interval = 5;
      SD_RemoveFile (SD_OB10M_FILE);
      SD_RemoveFile (SD_OB15M_FILE);
    }
    else if (SD.exists(SD_OB10M_FILE)) {
      Output ("OBI10M Found");
      obs_interval = obs_tx_interval = 10;
      SD_RemoveFile (SD_OB15M_FILE);
    }
    else if (SD.exists(SD_OB15M_FILE)) {
      Output ("OBI15M Found");
      obs_interval = obs_tx_interval = 15;
    }
    else {
      obs_interval = DEFAULT_OBS_INTERVAL;
      obs_tx_interval = DEFAULT_OBS_TRANSMIT_INTERVAL;
    }
  }

  // Do a check and make sure OBS and Transmit is at least 5m or greater when AQS is enabled
  if (AQS_Enabled) {
    if (obs_interval<5) {
      Output ("OBI Corrected 5M");
      obs_interval = 5;
    }
    if (obs_tx_interval<5) {
      Output ("TXI Corrected 5M");
      obs_tx_interval = 5;
    }
  }

  sprintf (msgbuf, "OBI=%dM, TXI=%dM", (int) obs_interval, (int) obs_tx_interval);
  Output(msgbuf);  
}


