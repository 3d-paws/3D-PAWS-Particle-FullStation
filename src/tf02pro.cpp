/*
 * ======================================================================================================================
 *  tf02pro.cpp - TF02Pro TOF Distance Sensor Functions
 * ======================================================================================================================
 */
#include "include/qc.h"
#include "include/output.h"
#include "include/obs.h"
#include "include/main.h"
#include "include/tf02pro.h"

/*
 * ======================================================================================================================
 * Variables and Data Structures
 * =======================================================================================================================
 */
TF02ProI2C *tf02pro = nullptr;
bool tf02pro_exists = false;

/* 
 *=======================================================================================================================
 * tf02pro_TakeReading() -- 
 *=======================================================================================================================
 */
bool tf02pro_TakeReading(uint16_t &d, uint16_t &s, float &t) {
  return (tf02pro->readFrame(d, s, t, 150)); // 150ms timeout
}

/* 
 *=======================================================================================================================
 * tf02pro_readAvg() -- returns the average
 *=======================================================================================================================
 */
bool tf02pro_readAvg(float &d, float &s, float &t) {
  uint16_t dist;
  uint16_t strength;
  float tempC;

  d = s = t = .0F;

  for (int i=0; i<TF02PRO_SAMPLES; i++) {
    if (!tf02pro->readFrame(dist, strength, tempC, 150)) {
      d = s = t = -999.9F;
      return(false);
    }
    d += dist;
    s += strength;
    t += tempC;
    delay (10);
  }

  d = d / TF02PRO_SAMPLES;
  s = s / TF02PRO_SAMPLES;
  t = t / TF02PRO_SAMPLES;

  d = (isnan(d) || (d < 5.0F)     || (d > 4000.0F))  ? -999.9F  : d;
  s = (isnan(s) || (s < .0F)      || (s > 65535.0F)) ? -999.9F  : s;
  t = (isnan(t) || (t < QC_MIN_T) || (t > QC_MAX_T)) ? QC_ERR_T : t;

  return (true);
}

/* 
 *=======================================================================================================================
 * tf02pro_obs_do() - do obs for TF02Pro device
 *=======================================================================================================================
 */
void tf02pro_obs_do(int oidx, int sidx) {
  if (tf02pro_exists) {
    float d,s,t;

    tf02pro_readAvg(d, s, t); 
\
    strcpy (obs[oidx].sensor[sidx].id, "t02d");
    obs[oidx].sensor[sidx].type = F_OBS;
    obs[oidx].sensor[sidx].f_obs = (float) d;
    obs[oidx].sensor[sidx++].inuse = true;

    strcpy (obs[oidx].sensor[sidx].id, "t02s");
    obs[oidx].sensor[sidx].type = F_OBS;
    obs[oidx].sensor[sidx].f_obs = (float) s;
    obs[oidx].sensor[sidx++].inuse = true;

    strcpy (obs[oidx].sensor[sidx].id, "t02t");
    obs[oidx].sensor[sidx].type = F_OBS;
    obs[oidx].sensor[sidx].f_obs = (float) t;
    obs[oidx].sensor[sidx++].inuse = true;    
  }
}

/* 
 *=======================================================================================================================
 * tf02pro_initialize() - initialize TF02Pro TOF Distance Senso
 *=======================================================================================================================
 */
void tf02pro_initialize() {
  Output("tf02pro:INIT");

  tf02pro = new TF02ProI2C(Wire, TF02PRO_ADDRESS);
  tf02pro_exists = tf02pro->begin();

  if (tf02pro_exists) { 
    uint16_t dist, strength;
    float tempC;
    if (tf02pro->readFrame(dist, strength, tempC, 150)) { // 150ms timeout
      Output ("tf02pro:OK");
      sprintf (Buffer32Bytes, "  Dist: %d", dist);
      Output(Buffer32Bytes);
      sprintf (Buffer32Bytes, "  Strength: %d", strength);
      Output(Buffer32Bytes);
      sprintf (Buffer32Bytes, "  Temp: %.2f", tempC);
      Output(Buffer32Bytes);
    }
    else {
      tf02pro_exists = false;
      Output ("tf02pro:ERROR");
    }
  }
  else {
    Output ("tf02pro:NF");
  }
}