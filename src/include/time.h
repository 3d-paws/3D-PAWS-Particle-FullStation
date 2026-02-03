/*
 * ======================================================================================================================
 *  time.h - Time Management Definations
 * ======================================================================================================================
 */
#if (PLATFORM_ID == PLATFORM_MSOM)
#define AB1805_ADDRESS 0x69  // AM1805 RTC/Watchdog

#include <AB1805_RK.h>    // On board WatchDog Power Management
#include <location.h>     // from particle-som-gnss library
#else
#include <RTClib.h>
// #include <Adafruit_ADS1X15.h>
#endif


// Extern variables
#if (PLATFORM_ID == PLATFORM_MSOM)
extern AB1805 ab1805;
extern struct tm now;
#else
extern RTC_PCF8523 rtc;
extern DateTime now;
#endif

extern char timestamp[128];
extern bool RTC_valid;
extern bool RTC_exists;

// Function prototypes
void stc_timestamp();
void rtc_timestamp();
void rtc_initialize();
void RTC_UpdateCheck();
