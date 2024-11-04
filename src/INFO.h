/*
 * ======================================================================================================================
 *  INFO.h - Report Information about the station at boot and when requested by DoAction on Particle Console
 * ======================================================================================================================
 */

/*
  System.uptime()
*/

/*
 * ======================================================================================================================
 * INFO_Do() - Get and Send System Information to Particle Cloud
 * ======================================================================================================================
 */
bool INFO_Do() {
  char buf[256];
  const char *comma = "";
  time_t ts = Time.now();

  Output("INFO_DO()");

  SendSystemInformation = false;

  memset(buf, 0, sizeof(buf));
  memset(msgbuf, 0, sizeof(msgbuf));  // hold the json formatted message we are building for sending.

  JSONBufferWriter writer(msgbuf, sizeof(msgbuf)-1);
  writer.beginObject();

  writer.name("devid").value(System.deviceID());
  writer.name("devos").value(System.version());
  writer.name("freemem").value(System.freeMemory());
  writer.name("uptime").value(System.uptime());

#if PLATFORM_ID == PLATFORM_ARGON
  writer.name("type").value("argon");
#endif
#if PLATFORM_ID == PLATFORM_BORON
  writer.name("board").value("boron");
#endif

  sprintf (Buffer32Bytes, "%d-%02d-%02dT%02d:%02d:%02d",
    Time.year(ts), Time.month(ts), Time.day(ts),
    Time.hour(ts), Time.minute(ts), Time.second(ts));
  writer.name("at").value(Buffer32Bytes);

  writer.name("ver").value(VERSION_INFO);
  writer.name("hth").value((int) SystemStatusBits);

  sprintf (Buffer32Bytes,"%ds", OBSERVATION_INTERVAL/1000);
  writer.name("obsi").value(Buffer32Bytes);
  sprintf (Buffer32Bytes,"%dm", obs_tx_interval);
  writer.name("obsti").value(Buffer32Bytes);
  // Tine 2 Next Transmit in Seconds
  sprintf (Buffer32Bytes, "%ds", (obs_tx_interval * 60) - (int)((System.millis() - LastTransmitTime)/1000));
  writer.name("t2nt").value(Buffer32Bytes);

  // Need 2 Send File
  if (SD.exists(SD_n2s_file)) {
    File fp = SD.open(SD_n2s_file, FILE_WRITE);
    if (fp) {
      writer.name("n2s").value(fp.size());
      fp.close();
    }
    else {
      writer.name("n2s").value(-1);
    }
  }
  else {
    writer.name("n2s").value("NF");
  }

#if PLATFORM_ID == PLATFORM_ARGON
  writer.name("ps").value((digitalRead(PWR)) ? "USB" : "BATTERY");
  writer.name("bv").value(analogRead(BATT) * 0.0011224); // Battery Voltage
  writer.name("bcs").value((digitalRead(PWR) && !digitalRead(CHG)) ? "CHARGING" : "!CHARGING"); // Battery Charger State
#endif

#if PLATFORM_ID == PLATFORM_BORON
  // Power Source
  char *ps[] = {"UNKN", "VIN", "USB_HOST", "USB_ADAPTER", "USB_OTG", "BATTERY"};
  int sps = System.powerSource();
  if ((sps>=0) && (sps<=5)) {
    writer.name("ps").value(ps[sps]);
  }
  else {
    writer.name("ps").value(sps);
  }

  // Battery Charge State
  char *bs[] = {"UNKN", "!CHARGING", "CHARGING", "CHARGED", "DISCHARGING", "FAULT", "MISSING"};
  int sbs = System.batteryState();
  if ((sbs>=0) && (sbs<=6)) {
    writer.name("bcs").value(bs[sbs]);     
  }
  else {
    writer.name("bcs").value(sbs);
  }
  writer.name("bpc").value(System.batteryCharge(), 1);   // Battery Percent Charge
#endif

#if PLATFORM_ID == PLATFORM_ARGON
  WiFiSignal sig = WiFi.RSSI();
  writer.name("wss").value(sig.getStrength(), 4);
  writer.name("wsq").value(sig.getQuality(), 4);
  byte mac[6];
  WiFi.macAddress(mac);
  sprintf (Buffer32Bytes, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  writer.name("mac").value(Buffer32Bytes);
  writer.name("ip").value(WiFi.localIP().toString().c_str());
  writer.name("mask").value(WiFi.subnetMask().toString().c_str());
  writer.name("gateway").value(WiFi.gatewayIP().toString().c_str());
  writer.name("dns").value(WiFi.dnsServerIP().toString().c_str());
  writer.name("dhcps").value(WiFi.dhcpServerIP().toString().c_str());
  writer.name("ssid").value(WiFi.SSID());
  WiFi.BSSID(mac);
  sprintf (Buffer32Bytes, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  writer.name("bssid").value(Buffer32Bytes);
#endif

#if PLATFORM_ID == PLATFORM_BORON
  CellularSignal sig = Cellular.RSSI();
  writer.name("css").value(sig.getStrength(), 4);
  writer.name("csq").value(sig.getQuality(), 4);

  writer.name("imsi").value(imsi); // International Mobile Subscriber Identity

  SimType simType = Cellular.getActiveSim();
  if (simType == INTERNAL_SIM) {
    writer.name("actsim").value("INTERNAL");
  } else if (simType == EXTERNAL_SIM) {
    writer.name("actsim").value("EXTERNAL");
  } else {
    writer.name("actsim").value("ERR");
  }
#endif

  // How Pin A4 is Configured
  if (A4_State == A4_STATE_DISTANCE) {
    if (dg_adjustment == 1.25) {
      writer.name("a4").value("DIST 5M");
    }
    else {
      writer.name("a4").value("DIST 10M");
    }
  }
  else if (A4_State == A4_STATE_RAIN){
    writer.name("a4").value("RG2");
  }
  else {
    writer.name("a4").value("NS"); // Not Set
  }

  // Sensors
  if (BMX_1_exists) {
    sprintf (buf+strlen(buf), "%sBMX1(%s)", comma, bmxtype[BMX_1_type]);
    comma=",";
  }
  if (BMX_2_exists) {
    sprintf (buf+strlen(buf), "%sBMX2(%s)", comma, bmxtype[BMX_2_type]);
    comma=",";
  }
  if (MCP_1_exists) {
    sprintf (buf+strlen(buf), "%sMCP1", comma);
    comma=",";
  }
  if (MCP_2_exists) {
    sprintf (buf+strlen(buf), "%sMCP2", comma);
    comma=",";
  }
  if (MCP_3_exists) {
    sprintf (buf+strlen(buf), "%sMCP3/gt1", comma);
    comma=",";
  }
  if (MCP_4_exists) {
    sprintf (buf+strlen(buf), "%sMCP4/gt2", comma);
    comma=",";
  }
  if (SHT_1_exists) {
    sprintf (buf+strlen(buf), "%sSHT1", comma);
    comma=",";
  }
  if (SHT_2_exists) {
    sprintf (buf+strlen(buf), "%sSHT2", comma);
    comma=",";
  }
  if (HDC_1_exists) {
    sprintf (buf+strlen(buf), "%sHDC1", comma);
    comma=",";
  }
  if (HDC_2_exists) {
    sprintf (buf+strlen(buf), "%sHDC2", comma);
    comma=",";
  }
  if (HIH8_exists) {
    sprintf (buf+strlen(buf), "%sHIH8", comma);
    comma=",";
  }
  if (SI1145_exists) {
    sprintf (buf+strlen(buf), "%sSI", comma);
    comma=",";
  }
  if (VEML7700_exists) {
    sprintf (buf+strlen(buf), "%sVEML", comma);
    comma=",";
  }
  if (AS5600_exists) {
    sprintf (buf+strlen(buf), "%sAS5600", comma);
    comma=",";
  }
  if (HI_exists) {
    sprintf (buf+strlen(buf), "%sHI", comma);
    comma=",";
  }
  if (WBT_exists) {
    sprintf (buf+strlen(buf), "%sWBT", comma);
    comma=",";
  }
  if (WBGT_exists) {
    if (MCP_3_exists) {
      sprintf (buf+strlen(buf), "%sWBGT W/GLOBE", comma);
    }
    else {
      sprintf (buf+strlen(buf), "%sWBGT WO/GLOBE", comma);
    }
    
    comma=",";
  }
  if (PM25AQI_exists) {
    sprintf (buf+strlen(buf), "%sPM25AQ", comma);
    comma=",";
  }
  writer.name("sensors").value(buf);

  // LoRa
  if (LORA_exists) {
    sprintf (buf, "%d,%d,%dMHz", cf_lora_unitid, cf_lora_txpower, cf_lora_freq);
    writer.name("lora").value(buf);
  }

  // Oled Display
  if (oled_type) {
    writer.name("oled").value(OLED32 ? "32" : "64");
  }
  else {
    writer.name("oled").value("NF");
  }
  writer.name("scepin").value((digitalRead(SCE_PIN)) ? "DISABLED" : "ENABLED");
  writer.name("sce").value((SerialConsoleEnabled) ? "TRUE" : "FALSE");

  writer.endObject();

  if (Particle_Publish((char *) "INFO")) {
    Serial_write (msgbuf);
    sprintf (Buffer32Bytes, "INFO->PUB OK[%d]", strlen(msgbuf)+1);
    Output(Buffer32Bytes);
    return(true);
  }
  else {
    sprintf (Buffer32Bytes, "INFO->PUB ERR");
    Output(Buffer32Bytes);
    return(false);
  }
}