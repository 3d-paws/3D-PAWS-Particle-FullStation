/*
 * ======================================================================================================================
 *  main.h - Main Code Definations
 * ======================================================================================================================
 */
#include <Particle.h>

/*
 * ======================================================================================================================
 *  Loop Timers
 * ======================================================================================================================
 */
#define DELAY_NO_RTC               60000    // Loop delay when we have no valided RTC
#define DEFAULT_OBS_INTERVAL           1    // 1 minute
#define DEFAULT_OBS_TRANSMIT_INTERVAL 15    // Transmit observations every N minutes Set to 15 for 15min Transmits

/*
 * ======================================================================================================================
 *  Relay Power Control Pin
 * ======================================================================================================================
 */
#if PLATFORM_ID == PLATFORM_MSOM
#define REBOOT_PIN            D27  // Trigger Watchdog or external relay to cycle power
#else
#define REBOOT_PIN            A0  // Trigger Watchdog or external relay to cycle power
#define HEARTBEAT_PIN         A1  // Watchdog Heartbeat Keep Alive
#endif

#define MAX_MSGBUF_SIZE 1024

// Extern variables
extern char versioninfo[];
extern char msgbuf[MAX_MSGBUF_SIZE];
extern char *msgp;
extern char Buffer32Bytes[32];
extern int LED_PIN;
extern bool JustPoweredOn;
extern bool TurnLedOff;
extern bool SendSystemInformation;
extern uint64_t obs_interval;
extern uint64_t obs_tx_interval;
extern uint64_t LastTransmitTime;
extern int DailyRebootCountDownTimer;
extern uint64_t lastOBS;
extern uint64_t LastTimeUpdate;

#if (PLATFORM_ID == PLATFORM_BORON) || (PLATFORM_ID == PLATFORM_MSOM)
extern PMIC pmic; // Power Management IC (bq24195) I2C 0x6B
#else
// extern FuelGauge fuel;  // Fuel Gauge IC (MAX17043) I2C 0x36
#endif

#if (PLATFORM_ID == PLATFORM_MSOM)
extern bool MuonWifiEnabled;
#endif

 // Function prototypes
 void BackGroundWork();
 void HeartBeat();