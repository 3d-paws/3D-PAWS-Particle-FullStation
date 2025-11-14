/*
 * ======================================================================================================================
 *  evt.cpp - Evapotranspiration Functions
 * ======================================================================================================================
 */
#include "include/qc.h"
#include "include/main.h"
#include "include/output.h"
#include "include/cf.h"
#include "include/sensors.h"
#include "include/wrda.h"
#include "include/sdcard.h"
#include "include/obs.h"
#include "include/evt.h"

#ifdef ENABLE_Evapotranspiration
/*
 * ======================================================================================================================
 * Variables and Data Structures
 * =======================================================================================================================
 */

// Note: static below means we are to limit this variable to this file.

// -------------------- Constants --------------------
static constexpr double SIGMA_MJ = 4.903e-9;   // Stefan–Boltzmann [MJ K^-4 m^-2 h^-1]
static constexpr double G_SC = 0.0820;         // Solar constant [MJ m^-2 min^-1]
static constexpr double PI_ = 3.14159265358979323846;

// -------------------- Config variables from CONFIG.TXT and ELEV.TXT --------------------
/*
 * int cf_elevation  
 * float cf_lat_deg
 * float cf_lon_deg
 * float cf_albedo
 * float cf_crop_kc
 * float cf_sr_cal
 * float cf_sr_dark_offset
 */

// -------------------- Hourly accumulator --------------------
struct EvtHour {
  int   hour_key   = -1;      // YYYYDOYHH
  int   n          = 0;       // # of minute samples in the hour
  double T_sum     = 0.0;     // °C
  double RH_sum    = 0.0;     // %
  double u2_sum    = 0.0;     // m/s
  double Rs_MJ     = 0.0;     // MJ m^-2 (sum over the hour)
};
EvtHour acc;

struct SolarHour {
  double Ra_MJ;   // Extraterrestrial radiation for the hour [MJ m^-2 h^-1]
  bool   isDay;
};

// -------------------- Sample and Report Timers --------------
int evt_min = -1;  // if current min != evt_min, take sample and update evt_lastmin with minute

/*
 * ======================================================================================================================
 *  Adafruit ADS1115 4-Channel ADC Breakout used with SP Lite2 Pyranometer from Kipp & Zonen
 * ======================================================================================================================
 */
Adafruit_ADS1115 ads;
bool ADS_exists = false;
bool EVT_exists = false;

/*
 * ======================================================================================================================
 * evt_readIrradiance() - via ADS with SP Lite2 Pyranometer from Kipp & Zonen on 0 & 1 ports
 *
 *  Reported as sr = shortwave radiation (what the pyranometer is detecting)
 * ======================================================================================================================
 */
float evt_readIrradiance(int samples = 16) {

  // cf_sr_cal aka CAL_SENS_uV_PER_WM2
  // Replace with your sensor's calibration constant (µV per W/m² from the label/certificate)
  // Check the calibration sticker on your SP Lite2 — it will say something like: Sensitivity: 74.8 µV per W/m²

  // cf_sr_dark_offset
  // With the sensor covered, log the raw counts, set that as dark_offset_counts if needed.
  // The "dark offset" is the baseline reading from the sensor when no irradiance (solar radiation) is 
  // present—essentially, the background signal or noise level that the pyranometer outputs under fully 
  // covered or dark conditions.

  // ADS1115 at GAIN = 16: LSB = 7.8125 µV per count
  const float LSB_uV = 7.8125;

  long sum = 0;
  for (int i = 0; i < samples; i++) {
    int16_t raw = ads.readADC_Differential_0_1();
    sum += raw;
    delay(5);
  }

  float avg_counts = (float)sum / samples;
  avg_counts -= cf_sr_dark_offset;  // subtract dark offset if measured

  // Counts → microvolts
  float V_uV = avg_counts * LSB_uV;

  // Irradiance (W/m²)
  float irradiance = V_uV / cf_sr_cal;
  if (irradiance < 0) irradiance = 0;  // clamp to zero

  return irradiance;
}

/*
 * ======================================================================================================================
 * dayOfYear() -
 * =======================================================================================================================
 */
static inline int dayOfYear(time_t ts) {
    int year = Time.year(ts);
    int month = Time.month(ts);
    int day = Time.day(ts);

    // Determine leap year inline
    bool isLeapYear = (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear) {
        daysInMonth[1] = 29;
    }
    int dayCount = 0;
    for (int i = 0; i < month - 1; i++) {
        dayCount += daysInMonth[i];
    }
    dayCount += day;
    return dayCount;
}


/*
 * ======================================================================================================================
 * esat_kPa() -
 * =======================================================================================================================
 */
static inline double esat_kPa(double T_C) {
  // Tetens (FAO-56): kPa
  return 0.6108 * exp((17.27 * T_C) / (T_C + 237.3));
}

/*
 * ======================================================================================================================
 * ea_kPa_from_RH() -
 * =======================================================================================================================
 */
static inline double ea_kPa_from_RH(double T_C, double RH_pct) {
  return (RH_pct/100.0) * esat_kPa(T_C);
}

/*
 * ======================================================================================================================
 * delta_slope_kPa_perC() -
 * =======================================================================================================================
 */
static inline double delta_slope_kPa_perC(double T_C) {
  double es = esat_kPa(T_C);
  return 4098.0 * es / pow(T_C + 237.3, 2.0);
}

/*
 * ======================================================================================================================
 * pressure_kPa_from_elev() -
 * =======================================================================================================================
 */
static inline double pressure_kPa_from_elev(double z_m) {
  // FAO-56 Eq. 7
  return 101.3 * pow(((293.0 - 0.0065*z_m)/293.0), 5.26);
}

/*
 * ======================================================================================================================
 * gamma_kPa_perC() -
 * =======================================================================================================================
 */
static inline double gamma_kPa_perC(double P_kPa) {
  // FAO-56: gamma = 0.000665 * P
  return 0.000665 * P_kPa;
}

/*
 * ======================================================================================================================
 * toRad() -
 * =======================================================================================================================
 */
static inline double toRad(double deg) { 
  return deg * PI_/180.0; 
}

/*
 * ======================================================================================================================
 * toDeg() -
 * =======================================================================================================================
 */
static inline double toDeg(double rad) { 
  return rad * 180.0/PI_; 
}

/*
 * ======================================================================================================================
 * equation_of_time_m() - Equation of Time (minutes), FAO-56 approximation
 * =======================================================================================================================
 */
static inline double equation_of_time_min(int DOY) {
  double B = 2.0*PI_*(DOY-81)/364.0;
  return 9.87*sin(2*B) - 7.53*cos(B) - 1.5*sin(B);
}

/*
 * ======================================================================================================================
 * hourly_Ra_MJ() -
 * =======================================================================================================================
 */
static inline SolarHour hourly_Ra_MJ(time_t ts, double lat_deg, double lon_deg) {
  // Build solar time interval for this hour (centered on the hour)
  int doy = dayOfYear(ts);
  int hr  = Time.hour(ts);
  int mn  = Time.minute(ts);

  // Convert clock to solar time (minutes from local midnight)
  // FAO-56 correction uses longitude and Eq. of Time (no timezone needed here since we want solar time).
  double EoT = equation_of_time_min(doy);    // minutes
  // Local solar time in minutes from midnight:
  // LSTM term (time zone meridian) would be needed if converting local clock; we assume UTC clock here.
  // Use longitude correction directly: 4*lon_deg (minutes), sign naturally handled by lon sign.
  double solar_minutes = hr*60.0 + mn + EoT + 4.0*lon_deg;

  // Hour boundaries in solar time (start & end of the current clock hour)
  double start_min = floor(solar_minutes/60.0)*60.0;   // start of the hour (solar minutes)
  double end_min   = start_min + 60.0;

  // Convert minutes to hour angles (omega) in radians: 15° per hour => 15 deg per 60 min => 0.25 deg/min
  auto minutes_to_omega = [](double m) { return toRad(0.25 * (m - 720.0)); }; // 720 min = solar noon

  double omega1 = minutes_to_omega(start_min);
  double omega2 = minutes_to_omega(end_min);

  // Declination (rad), inverse relative distance dr (FAO-56)
  double dr = 1.0 + 0.033 * cos(2.0*PI_*doy/365.0);
  double delta = 0.409 * sin(2.0*PI_*doy/365.0 - 1.39);
  double phi = toRad(lat_deg);

  // Sunset hour angle
  double ws_arg = -tan(phi) * tan(delta);
  if (ws_arg < -1.0) ws_arg = -1.0;
  if (ws_arg >  1.0) ws_arg =  1.0;
  double ws = acos(ws_arg);

  // Clip omega bounds to daylight interval [-ws, +ws]
  double om1 = max(-ws, min(ws, omega1));
  double om2 = max(-ws, min(ws, omega2));

  SolarHour out{};
  if (om1 == om2) {
    out.Ra_MJ = 0.0;
    out.isDay = false;
    return out;
  }

  // Hourly Ra (MJ m^-2 h^-1) — FAO-56 Eq. for short intervals (Allen et al. 1998)
  // Ra (MJ/m2/min) = (12*60/PI)*Gsc*dr [cos(phi)cos(delta)(sin(om2)-sin(om1)) + (om2-om1)sin(phi)sin(delta)] / 60
  // Folding constants → direct hourly MJ term:
  double Ra = (12.0*60.0/PI_) * G_SC * dr *
              ( (cos(phi)*cos(delta))*(sin(om2) - sin(om1)) + (om2 - om1)*sin(phi)*sin(delta) );
  // Ra is MJ m^-2 per hour for this interval
  out.Ra_MJ = max(0.0, Ra);
  out.isDay = (out.Ra_MJ > 1e-6);
  return out;
}

/*
 * ======================================================================================================================
 * Rso_MJ() - Clear-sky radiation (Rso) [MJ m^-2 h^-1]
 * =======================================================================================================================
 */
static inline double Rso_MJ(double Ra_MJ, double elev_m) {
  return (0.75 + 2e-5 * elev_m) * Ra_MJ;
}

/*
 * ======================================================================================================================
 * Rns_MJ() - Net radiation
 * =======================================================================================================================
 */
static inline double Rns_MJ(double Rs_MJ, double albedo) {
  // Shortwave net
  return (1.0 - albedo) * Rs_MJ;
}

/*
 * ======================================================================================================================
 * Rnl_MJ() - Rnl uses average T_K for the hour; FAO-56 uses (Tmax^4 + Tmin^4)/2 daily.
 *            For hourly, we approximate with T^4 and the standard cloud/ea terms.
 * =======================================================================================================================
 */
static inline double Rnl_MJ(double T_C, double ea_kPa, double Rs_MJ, double Rso_MJ) {
  double T_K = T_C + 273.16;
  double f_cloud = 0.0;
  if (Rso_MJ <= 0.0) {
    f_cloud = 0.0; // night
  } else {
    double ratio = Rs_MJ / Rso_MJ;
    // bound ratio in [0, 1.0..1.2], typical FAO clip near [0,1]
    if (ratio < 0.0) ratio = 0.0;
    if (ratio > 1.0) ratio = 1.0;
    f_cloud = 1.35 * ratio - 0.35;
  }
  double f_vp = 0.34 - 0.14 * sqrt(max(0.0, ea_kPa));
  return SIGMA_MJ * (pow(T_K, 4.0)) * f_vp * f_cloud; // MJ m^-2 h^-1
}

/*
 * ======================================================================================================================
 * G_MJ() -
 * =======================================================================================================================
 */
static inline double G_MJ(double Rn_MJ, bool isDay) {
  // FAO-56 hourly rule-of-thumb
  return isDay ? 0.1 * Rn_MJ : 0.5 * Rn_MJ;
}


/*
 * ======================================================================================================================
 * penman_monteith_hourly_mm() - Penman–Monteith (hourly)
 * =======================================================================================================================
 */
static inline double penman_monteith_hourly_mm(
                       time_t ts,            // timestamp for this hour
                       double T_C,           // hourly mean °C
                       double RH_pct,        // hourly mean %
                       double u2_ms,         // hourly mean m/s at 2 m
                       double Rs_MJ          // hourly total MJ m^-2
                    ) {
  // Psychrometrics
  double P  = pressure_kPa_from_elev(cf_elevation);
  double gam = gamma_kPa_perC(P);
  double del = delta_slope_kPa_perC(T_C);

  // Vapor pressures
  double es = esat_kPa(T_C);
  double ea = ea_kPa_from_RH(T_C, RH_pct);
  double vpd = max(0.0, es - ea);

  // Radiation terms
  SolarHour S = hourly_Ra_MJ(ts, cf_lat_deg, cf_lon_deg);
  double Rso = Rso_MJ(S.Ra_MJ, cf_elevation);
  double Rns = Rns_MJ(Rs_MJ, cf_albedo);
  double Rnl = Rnl_MJ(T_C, ea, Rs_MJ, Rso);
  double Rn  = Rns - Rnl;                 // MJ m^-2 h^-1
  double G   = G_MJ(Rn, S.isDay);         // MJ m^-2 h^-1

  // Convert radiation to equivalent mm of water in formula’s numerator: factor 0.408 (MJ → mm)
  // FAO-56 hourly PM (short crop) — consistent with daily form but on an hourly basis.
  double num = 0.408 * del * (Rn - G) + gam * (37.0/(T_C + 273.0)) * u2_ms * vpd;
  double den = del + gam * (1.0 + 0.34 * u2_ms);

  double ET0_mm_per_h = (den > 0.0) ? (num / den) : 0.0;
  if (ET0_mm_per_h < 0.0) ET0_mm_per_h = 0.0; // no negative ET
  return ET0_mm_per_h;
}

/*
 * ======================================================================================================================
 * make_hour_key() - return int value representing year+doy+hour
 * =======================================================================================================================
 */
static inline int make_hour_key(time_t ts) {
  int y = Time.year(ts);
  int doy = dayOfYear(ts);
  int hh = Time.hour(ts);
  return y*100000 + doy*100 + hh; // YYYY DDD HH
}

/*
 * ======================================================================================================================
 * EVT_ComputeHourAndReset() - Compute ET0 for the completed hour, return ET0 and ETc (if kc != 1)
 * =======================================================================================================================
 */
static inline bool EVT_ComputeHourAndReset (EvtHour &acc, double &ET0_mm_h, double &ETc_mm_h) {
  if (acc.n <= 0) {
    return (false);
  }

  // Build a representative timestamp at the END of the hour we just completed:
  // acc.hour_key encodes YYYY DDD HH; create a time at HH:30 as "hour center".
  int y  = acc.hour_key / 100000;
  int rem= acc.hour_key % 100000;
  int doy= rem / 100;
  int hh = rem % 100;

  // Convert year + DOY + hour to a time_t (Particle lacks direct DOY constructor; approximate via Jan-1 + DOY-1)
  struct tm t;
  t.tm_year = y - 1900;
  t.tm_mon  = 0;
  t.tm_mday = 1;
  t.tm_hour = 0;
  t.tm_min  = 0;
  t.tm_sec  = 0;
  time_t jan1 = mktime(&t);
  time_t hour_center = jan1 + (doy-1)*86400 + hh*3600 + 1800; // HH:30 solar approx

  double T  = acc.T_sum  / acc.n;
  double RH = acc.RH_sum / acc.n;
  double u2 = acc.u2_sum / acc.n;
  double Rs = acc.Rs_MJ; // already hour-summed in MJ m^-2

  ET0_mm_h = penman_monteith_hourly_mm(hour_center, T, RH, u2, Rs);
  ETc_mm_h = ET0_mm_h * cf_crop_kc;

  acc = EvtHour{}; // reset for next hour
  return (true);
}

/*
 * ======================================================================================================================
 * EVT_Build_JSON() - Create JSON ET observation in msgbuf
 * ======================================================================================================================
 */
void EVT_Build_JSON(time_t ts, double ET0, double ETc) {
  char tstamp[32];
    
  memset(msgbuf, 0, sizeof(msgbuf));

  JSONBufferWriter writer(msgbuf, sizeof(msgbuf)-1);
  writer.beginObject();

  sprintf (tstamp, "%d-%02d-%02dT%02d:%02d:%02d",
    Time.year(ts), Time.month(ts), Time.day(ts),
    Time.hour(ts), Time.minute(ts), Time.second(ts));

  writer.name("at").value(tstamp);

  writer.name("et").value(ET0, 3);

  // fabs() calculates the absolute value of a floating-point number.
  // It returns the magnitude of a number, disregarding its sign. If the input is negative, 
  // it returns the positive equivalent; if the input is already positive, it returns the same positive value.
  if (fabs(cf_crop_kc - 1.0) > 1e-6) {
    writer.name("etc").value(ETc, 3);
  } 
  writer.endObject();
}

/*
 * ======================================================================================================================
 * evt_do() - Take Minute Sample, On hour Compute ET0, ETc, report all values involved
 *
 * Notes & assumptions
 *   Wind height: All stations are at 2 m  so no height correction.
 *   Soil heat flux (G): FAO-56 hourly guideline used (0.1·Rₙ in daylight, 0.5·Rₙ at night).
 *   Net longwave (Rnl): Uses hourly approximation with a single T (K) and the clearness term from Rs/Rso.
 *   Solar time: We apply the equation of time and longitude to approximate solar hour angles (good accuracy for hourly ET₀).
 *   Units: Output ET₀ is mm·h⁻¹. Summing 24 hours gives daily ET₀ (mm·d⁻¹).
 *   ETc: If you set crop_kc ≠ 1, we also report ETc = Kc·ET₀.
 * =======================================================================================================================
 */
void evt_do () {
  if (EVT_exists) {
    time_t ts = Time.now();
    int min  = Time.minute(ts);

    if (evt_min != min) {
      int key = make_hour_key(ts);

      // Initialize from startup
      if (acc.hour_key < 0) {
        acc.hour_key = key;
      }

      if (key != acc.hour_key && acc.n > 0) {
        Output("EVTHR:DO");
        double ET0, ETc;
        EVT_ComputeHourAndReset(acc, ET0, ETc); // Compute hour observation and reset acc
        EVT_Build_JSON(ts, ET0, ETc);
        SD_LogObservation(msgbuf);

        if (Particle_Publish((char *) "ET")) {
          Serial_write (msgbuf);
          sprintf (Buffer32Bytes, "ET->PUB OK[%d]", strlen(msgbuf)+1);
          Output(Buffer32Bytes);
        }
        else {
          Output("ET->PUB ERR");
          sprintf (msgbuf+strlen(msgbuf), ",ET");  // Add Particle Event Type after JSON structure
          SD_NeedToSend_Add(msgbuf); // Save to N2F File
          Output("ET Add N2S");
          Serial_write (msgbuf);
        }
      }

      // Read Sesonsors 
      double T_C = sht1.readTemperature();
      T_C = (isnan(T_C) || (T_C < QC_MIN_T)  || (T_C > QC_MAX_T))  ? QC_ERR_T  : T_C;

      double RH_pct = sht1.readHumidity();
      RH_pct = (isnan(RH_pct) || (RH_pct < QC_MIN_RH) || (RH_pct > QC_MAX_RH)) ? QC_ERR_RH : RH_pct;

      if ((T_C!=QC_ERR_T) && (RH_pct !=QC_ERR_RH)) {
        double u2_ms  = Wind_SpeedAverage();  // m/s @ 2m (already so)
        double sr_Wm2 = evt_readIrradiance(); // W/m2

        acc.T_sum  += T_C;
        acc.RH_sum += RH_pct;
        acc.u2_sum += u2_ms;
        acc.Rs_MJ  += max(0.0, sr_Wm2) * 60.0 / 1e6;   // 1 minute integration
        acc.n++;
        Output("EVT1M:OK");
      }
      else {
        Output("EVT1M:ERR");
      }
      evt_min = min;
    }
  }
}

/*
 * ======================================================================================================================
 *  evt_initialize() - Initialize 4-Channel ADC Breakout used with SP Lite2 Pyranometer from Kipp & Zonen
 *                     Checks for shortwave radiation from pyranometer via 16bit A/D sets ADS_exists.
                       Requires SHT_1_exists & AS5600_exists to be true for Evapotranspiration to run, sets EVT_exists
 * ======================================================================================================================
 */
void evt_initialize() {
  Output("EVT:INIT");

  if (!ads.begin()) {
    ADS_exists = false;
    Output ("EVT:NF");
  }
  else { 
    ads.setGain(GAIN_SIXTEEN);
    ads.readADC_Differential_0_1(); // read once and toss result
    ADS_exists = true;
    Output ("ADS:OK");
    if (SHT_1_exists && AS5600_exists) {
      EVT_exists = true;
      Output ("EVT:OK");      
    }
    else {
      Output ("EVT:NF");
    }
  }
}
#endif