/*
 * ======================================================================================================================
 *  tf02pro.h - TF02Pro TOF Distance Sensor Includes
 * 
 *  Sensor will report as
 *    t02d : distance cm - 10–4000 cm
 *    t02s : strength  -  0–65535 
 *    t02t : temperature C
 * 
 *  Strength Meaning:
 *    Higher = stronger return (better reflectivity / closer target / better alignment)
 *    Lower = weaker return (dark target, long range, glancing angle, etc.)
 * ======================================================================================================================
 */
#include <TF02ProI2C.h>

#define TF02PRO_ADDRESS 0x10      // Factory Default is 0x10
#define TF02PRO_SAMPLES 5         // How many samples taken for the average 

// Extern variables
extern TF02ProI2C *tf02pro;
extern bool tf02pro_exists;


// Function prototype
void tf02pro_initialize();
bool tf02pro_readAvg(float &d, float &s, float &t);
bool tf02pro_TakeReading(uint16_t &d, uint16_t &s, float &t);
void tf02pro_obs_do(int oidx, int sidx);