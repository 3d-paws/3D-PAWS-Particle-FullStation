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
#include "include/nvcf.h"
#include "include/ps.h"

const char *batterystate[] = {"UNKN", "!CHARGING", "CHARGING", "CHARGED", "DISCHARGING", "FAULT", "MISSING"};

volatile OTAState ota_state = NO_UPDATE;
unsigned long ota_stateStarted = 0;
const unsigned long UPDATE_WAIT_MS = 300000;        // 5 minutes

/*
 * ======================================================================================================================
 * Fuction Definations
 * =======================================================================================================================
 */

/*
 * ======================================================================================================================
 * enterOTAState() - 
 * ======================================================================================================================
 */
void enterOTAState(OTAState s) {
    ota_state = s;
    ota_stateStarted = millis();
}

#if (PLATFORM_ID == PLATFORM_BORON) || (PLATFORM_ID == PLATFORM_MSOM)
/*
 * ======================================================================================================================
 * getCellularGlobalIdentity() - 
 * 
 * Standard Decimal Format Max Length: 24 characters (including null-terminator) "999-999-65535-268435455\0"
 *   mcc: Max 3 characters (e.g., "999")
 *   mnc: Max 3 characters (e.g., "999")
 *   lac: Max 5 characters (e.g., "65535")
 *   cid: Max 9 characters (e.g., "268435455" for LTE/5G ECGI)
 * ======================================================================================================================
 */

bool getCellularGlobalIdentity(char *buf, size_t buf_len) {
  CellularGlobalIdentity cgi = {0};
  cgi.size = sizeof(CellularGlobalIdentity);
  cgi.version = CGI_VERSION_LATEST;

  cellular_result_t res = cellular_global_identity(&cgi, NULL);

  if (res != SYSTEM_ERROR_NONE) {
      buf[0] = '\0';
      return false;
  }

  snprintf(buf, buf_len, "%d-%d-%u-%lu",
    cgi.mobile_country_code,
    cgi.mobile_network_code,
    cgi.location_area_code,
    cgi.cell_id);
  return true;
}
#endif


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

  sprintf (Buffer32Bytes, "CS:%.2f", SignalStrength);
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
  
  sprintf (Buffer32Bytes, "CS:%.2f B:%d,%.2f", SignalStrength, BatteryState, BatteryPoC);
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

    else if (s.equals("CNV")) { // Clear NV Config Files
    Output("DoAction:CNV");
    return(nv_deleteConfigFiles());
  }

  else if (s.equals("NOWIND")) { 
    Output("DoAction:NOWIND");
    ws_refresh = false;
    scv.wind=false;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);  
  }

  else if (s.equals("DOWIND")) {
    Output("DoAction:DOWIND");
    scv.wind=true;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);  
  }

  else if (s.equals("NORAIN")) { 
    Output("DoAction:NORAIN");
    scv.rg1=false;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);  
  }

  else if (s.equals("DORAIN")) { 
    Output("DoAction:DORAIN");
    scv.rg1=true;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);  
  }

  else if (s.equals("OP1DIST")) { // Set OP1 State File to 10m Distance 
    Output("DoAction:OP1DIST");
    scv.op1 = OP1_STATE_DISTANCE;
    scv.op1d5m = false;
    dg_adjustment = 2.5;
    pinMode(OP1_PIN, INPUT);
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.equals("OP1D5M")) { // Set 5M Distance Sensor State File
    Output("DoAction:OP1D5M");
    scv.op1d5m = true;
    dg_adjustment = 1.25;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.equals("OP1RAIN")) { // Set OP1 State File to Rain
    Output("DoAction:OP1RAIN");
    scv.op1 = OP1_STATE_RAIN;
    scv.op1d5m = false;
    dg_adjustment = 2.5;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.equals("OP1RAW")) { // Set OP1 State File to Raw
    Output("DoAction:OP1RAW");
    scv.op1 = OP1_STATE_RAW;
    scv.op1d5m = false;
    dg_adjustment = 2.5;
    pinMode(OP1_PIN, INPUT);
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.equals("OP1CLR")) {
    Output("DoAction:OP1CLR");
    scv.op1 = OP1_STATE_NULL;
    scv.op1d5m = false;
    dg_adjustment = 2.5;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.equals("OP2RAW")) { // Set OP2 State File to Raw
    Output("DoAction:OP2RAW");
    scv.op2 = OP2_STATE_RAW;
    pinMode(OP2_PIN, INPUT);
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.equals("OP2VBV")) { // Set OP2 State File to Voltaic Battery Voltage
    Output("DoAction:OP2VBV");
    scv.op2 = OP2_STATE_VOLTAIC;
    pinMode(OP2_PIN, INPUT);
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }
    
  else if (s.equals("OP2CLR")) { // Clear OP2 State Files
    Output("DoAction:OP2CLR");
    scv.op2 = OP2_STATE_NULL;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.equals("TXI5M")) { // Set 1 Minute Observations, Transmit Interval to 5 Minutes
    Output("DoAction:TXI5M");
    sprintf (msgbuf, "CUR: OBI=%dM, TXI=%dM", (int) scv.obi, (int) scv.txi);
    Output(msgbuf);  
    scv.obi = DEFAULT_OBS_INTERVAL;
    scv.txi = 5;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.equals("TXI10M")) { // Set 1 Minute Observations, Transmit Interval to 10 Minutes
    Output("DoAction:TXI10M");
    sprintf (msgbuf, "CUR: OBI=%dM, TXI=%dM", (int) scv.obi, (int) scv.txi);
    Output(msgbuf);  
    scv.obi = DEFAULT_OBS_INTERVAL;
    scv.txi = 10;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.equals("TXI15M")) { // Set 1 Minute Observations, Transmit Interval to 15 Minutes, 
    Output("DoAction:TXI15M");
    sprintf (msgbuf, "CUR: OBI=%dM, TXI=%dM", (int) scv.obi, (int) scv.txi);
    Output(msgbuf);
    scv.obi = DEFAULT_OBS_INTERVAL;
    scv.txi = DEFAULT_OBS_TRANSMIT_INTERVAL;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.equals("OBI5M")) { // Set 5 Minute Observations, Transmit Interval to 5 Minutes
    Output("DoAction:OBI5M");
    sprintf (msgbuf, "CUR: OBI=%dM, TXI=%dM", (int) scv.obi, (int) scv.txi);
    Output(msgbuf); 
    scv.obi = scv.txi = 5;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR"); 
    return(0);
  }

  else if (s.equals("OBI10M")) { // Set 10 Minute Observations, Transmit Interval to 10 Minutes
    Output("DoAction:OBI10M");
    sprintf (msgbuf, "CUR: OBI=%dM, TXI=%dM", (int) scv.obi, (int) scv.txi);
    Output(msgbuf); 
    scv.obi = scv.txi = 10;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR"); 
    return(0);
  }

  else if (s.equals("OBI15M")) { // Set 15 Minute Observations, Transmit Interval to 15 Minutes
    Output("DoAction:OBI15M");
    sprintf (msgbuf, "CUR: OBI=%dM, TXI=%dM", (int) scv.obi, (int) scv.txi);
    Output(msgbuf);  
    scv.obi = scv.txi = 15;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.equals("OPTAQS")) { // Enable Air Quality Station
    Output("DoAction:OPTAQS");
    scv.aqs = true;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.equals("OPTFS")) { // Enable Full Station
    Output("DoAction:OPTFS");
    time32_t current_time = Time.now();
    EEPROM_ClearRainTotals(current_time);
    scv.aqs = false;
    (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
    return(0);
  }

  else if (s.startsWith("SETELEV:")) { // Pattern start of string aka 0 offset
    Output("DoAction:SETELEV");
    String rest = s.substring(8);   // get part after "SETELEV:", 8 = length of 
    long elevation = rest.toInt();  // convert to integer

    if ((String(elevation) == rest) && (elevation >= QC_MIN_ELEV) && (elevation <= QC_MAX_ELEV)) {
      scv.elevation = elevation; // Set running value of elevation
      mslp_initialize(); // Set flags so we don't need to reboot.
      (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
      return(0);
    }
    else {
      Output("SETELEV, INVALID ELEV#"); 
      return(-1);           
    }
  }

  else if (s.startsWith("SETRTRO:")) { // Pattern start of string aka 0 offset
    Output("DoAction:SETRTRO");
    int hour = 0;
    int minute = 0;
    bool valid = false;

    String rest = s.substring(8); // get part after "SETRTRO:", 8 = length of 

    // Trim whitespace
    rest = rest.trim();

    int parsed = sscanf(rest.c_str(), "%d:%d", &hour, &minute);

    if (parsed == 2) {
      // H:MM format - validate quarter-hour
      if ((hour >= 0 && hour <= 23) && (minute == 0 || minute == 15 || minute == 30 || minute == 45)) {
        valid=true;
      }
    } 
    else if (parsed == 1) {
      // Just H format (minute = 0)
      hour = atoi(rest.c_str());
      if (hour >= 0 && hour <= 23) {
        valid=true;
      }
    } 

    // Save to SD and set config values
    if (valid) {
      scv.rtro_hour = hour;
      scv.rtro_minute = minute;
      (saveConfig()) ? Output ("NVSave:OK") : Output ("NVSave:ERR");
      return (0);
    } 
    else {
      sprintf(Buffer32Bytes, "SETRTRO:%d:%02d INVALID", hour, minute);
      Output (Buffer32Bytes);
      return(-2);
    }
  }
  else {
    Output("DoAction:UKN"); 
    return(-1);
  }
}

#if PLATFORM_ID == PLATFORM_ARGON
/*
 * ======================================================================================================================
 * ARGON_network_initialize() - Set Wireless SSID, Password            
 * ======================================================================================================================
 */
void ARGON_network_initialize() { 
  if (scv.wifi_auth == "") {
    Output("WIFI:AUTH=Null Err");
    Output("WIFI:Credentials NOT Set");
    Output("WIFI:Use Existing");
  }
  else if ((scv.wifi_auth != "WEP") &&
           (scv.wifi_auth != "WPA") &&
           (scv.wifi_auth != "WPA2") &&
           (scv.wifi_auth != "UNSEC")) {
    sprintf (msgbuf, "WIFI:AUTH[%s] Error", scv.wifi_auth.c_str());          
    Output(msgbuf);
    Output("WIFI:Credentials NOT Set");
    Output("WIFI:Use Existing");
  }
  else if (scv.wifi_ssid == "") {
    Output("WIFI:SID NotSet");
    Output("WIFI:Credentials NOT Set");
    Output("WIFI:Use Existing");
  }
  else if ((scv.wifi_auth != "UNSEC") && (scv.wifi_pw == "")) {
    // UNSEC is allowed to have no password just a ssid, but non of the others
    Output("WIFI:PW NotSet");
    Output("WIFI:Credentials NOT Set");
    Output("WIFI:Use Existing");
  }
  else {
    sprintf (msgbuf, "WIFI:AUTH[%s]", scv.wifi_auth.c_str()); Output(msgbuf);
    sprintf (msgbuf, "WIFI:SSID[%s]", scv.wifi_ssid.c_str()); Output(msgbuf);
    sprintf (msgbuf, "WIFI:PW[%s]",   scv.wifi_pw.c_str());   Output(msgbuf);

    // Connects to a network secured with WPA2 credentials.
    // https://docs.particle.io/reference/device-os/api/wifi/securitytype-enum/
    if (scv.wifi_auth == "UNSEC") {
      Output("WIFI:Credentials Cleared");
      WiFi.clearCredentials();
      if (scv.wifi_pw == "") {
        Output("WIFI:Credentials Set UNSEC NO PW");
        WiFi.setCredentials(scv.wifi_ssid.c_str());
      }
      else {
        Output("WIFI:Credentials Set UNSEC");
        WiFi.setCredentials(scv.wifi_ssid.c_str(), scv.wifi_pw.c_str());                 
      }
    }
    else if (scv.wifi_auth == "WEP") {
      Output("WIFI:Credentials Cleared");
      WiFi.clearCredentials();
      Output("WIFI:Credentials Set WEP");
      WiFi.setCredentials(scv.wifi_ssid.c_str(), scv.wifi_pw.c_str(), WEP);
    }
    else if (scv.wifi_auth == "WPA") {
      Output("WIFI:Credentials Cleared");
      WiFi.clearCredentials();
      Output("WIFI:Credentials Set WPA");
      WiFi.setCredentials(scv.wifi_ssid.c_str(), scv.wifi_pw.c_str(), WPA);
    }
    else if (scv.wifi_auth == "WPA2") {
      Output("WIFI:Credentials Cleared");
      WiFi.clearCredentials();
      Output("WIFI:Credentials Set WPA2");
      WiFi.setCredentials(scv.wifi_ssid.c_str(), scv.wifi_pw.c_str(), WPA2);
    }
    else if (scv.wifi_auth == "WPA_ENTERPRISE") {
      // WPA Enterprise is only supported on the Photon and P1.
      // It is not supported on the Argon, P2, and Photon 2.
      Output("WIFI:Credentials Cleared");
      WiFi.clearCredentials();
      Output("WIFI:Credentials Set WPA ENT");
      WiFi.setCredentials(scv.wifi_ssid.c_str(), scv.wifi_pw.c_str(), WPA_ENTERPRISE);
    }
    else if (scv.wifi_auth == "WPA2_ENTERPRISE") {
      // WPA Enterprise is only supported on the Photon and P1.
      // It is not supported on the Argon, P2, and Photon 2.
      Output("WIFI:Credentials Cleared");
      WiFi.clearCredentials();
      Output("WIFI:Credentials Set WPA ENT2");
      WiFi.setCredentials(scv.wifi_ssid.c_str(), scv.wifi_pw.c_str(), WPA2_ENTERPRISE);
    }
    else { 
      Output("WIFI:Credentials NOT Set");
      Output("WIFI:Use Existing");
    }
  }
}
#endif

#if PLATFORM_ID == PLATFORM_MSOM
/*
 * ======================================================================================================================
 * MUON_network_initialize() - Setup WiFi if WIFI.TXT exists. Else setup Cellular            
 * ======================================================================================================================
 */
void MUON_network_initialize() {
  MuonWifiEnabled = false;

  if (scv.wifi_ssid == "") {
    Output("WIFI:SID NotSet");
    Output ("WIFO:USING CELLULAR");
  }
  else if (scv.wifi_pw == "") {
    Output("WIFI:PW NotSet");
    Output ("WIFO:USING CELLULAR");
  }
  else {
    Output("NETWORK:SET WIFI");
    MuonWifiEnabled = true;
    sprintf (msgbuf, "WIFI:SSID[%s]", scv.wifi_ssid.c_str()); Output(msgbuf);
    sprintf (msgbuf, "WIFI:PW[%s]",   scv.wifi_pw.c_str());   Output(msgbuf);

    Output("WIFI:Particle Cloud Disconnect");  // We should no be connected, but do anyway
    Particle.disconnect();

    Output("WIFI:Turning Off Cellular");
    Cellular.off();    // Turn off cellular modem
    waitUntil(Cellular.isOff);  // Optional: wait for cellular modem to power down

    Output("WIFI:Turning On Wifi");
    WiFi.on();
              
    if (WiFi.clearCredentials()) {
      Output("WIFI:Cleared Wifi Creds");
    } 
    else {
      Output("WIFI:Clear Wifi Creds Err");
    }

    if (WiFi.setCredentials(scv.wifi_ssid.c_str(), scv.wifi_pw.c_str())) {
      Output("WIFI:Credentials Set");
    } else {
      Output("WIFI:Credentials Set Err");
    }

    Output("WIFI:Connect Called");
    WiFi.connect();
    // waitUntil(WiFi.ready);  // No we want to move on with out network
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


