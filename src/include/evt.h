/*
 * ======================================================================================================================
 *  evt.h - Evapotranspiration Functions
 * ======================================================================================================================
 */

#if (PLATFORM_ID != PLATFORM_MSOM)
#define ENABLE_Evapotranspiration
#endif

#ifdef ENABLE_Evapotranspiration
#include <Adafruit_ADS1X15.h>

/*
 * ======================================================================================================================
 *  Adafruit ADS1115 4-Channel ADC Breakout used with SP Lite2 Pyranometer from Kipp & Zonen
 * 
 *  Single-ended mode: Pin N vs GND
 *  readADC_SingleEnded(channel), passing 0–3 for the channel. This measures the voltage between the specified analog 
 *     input (A0–A3) and ground. The raw result is typically reported as an unsigned integer (0 to 65535 for 16-bits), 
 *     but only positive voltages are measured, effectively using 15 bits of meaningful range.
 * 
 *  Differential mode: Pin N vs Pin M
 *  readADC_Differential_0_1() or readADC_Differential_2_3() where the pair selects which pins to use (for example, 
 *     A0 minus A1, or A2 minus A3). Here, the full 16-bit signed range is used, so results can be negative 
 *     (−32768 to +32767) because the device measures the voltage difference between two pins.
 * 
 *   ads.computeVolts(adc0) function applies the necessary scale factor (based on the ADS1115 gain setting) to convert 
 *     the raw 16-bit integer into a voltage in volts.
 * 
 *   Gain Settings
 *   The ADC input range (or gain) can be changed via the following functions, but be careful never to exceed 
 *   VDD +0.3V max, or to exceed the upper and lower limits if you adjust the input range!
 *                                                                  ADS1015  ADS1115
 *                                                                  -------  -------
 *   ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
 *   ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
 *   ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
 *   ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
 *   ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
 *   ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
 * ======================================================================================================================
 */

// Extern variables
extern Adafruit_ADS1115 ads;
extern bool ADS_exists;
extern bool EVT_exists;

// Function prototype
void evt_initialize();
float evt_readIrradiance(int samples);
void evt_do();
#endif