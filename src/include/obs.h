/*
 * ======================================================================================================================
 *  obs.h - Observation Definations
 * ====================================================================================================================== 
 */
#if (PLATFORM_ID == PLATFORM_MSOM)
#include <AB1805_RK.h>    // On board WatchDog Power Management
#else
#include <RTClib.h>
#endif

#define MAX_SENSORS         96
#define MAX_ONE_MINUTE_OBS  17 // Want more OBS space than our OBSERVATION_TRANSMIT_INTERVAL (For 15m interval use 17)
                              // This prevents OBS from filling and being written to N2S file while we are Connecting
#define TAGSZ 11

typedef enum {
  F_OBS, 
  I_OBS, 
  U_OBS
} OBS_TYPE;

typedef struct {
  char          id[TAGSZ+1];       // Suport 11 character length observation names
  int           type;
  float         f_obs;
  int           i_obs;
  unsigned long u_obs;
  bool          inuse;
} SENSOR;

typedef struct {
  bool            inuse;                // Set to true when an observation is stored here         
  time_t          ts;                   // TimeStamp
  float           css;                  // Cell Signal Strength
  unsigned long   hth;                  // System Status Bits
  SENSOR          sensor[MAX_SENSORS];
} OBSERVATION_STR;

// Extern variables
extern OBSERVATION_STR obs[MAX_ONE_MINUTE_OBS];

// Function prototypes
void OBS_Clear(int i);
void OBS_Init();
void OBS_Do();
void OBS_PublishAll();
bool Particle_Publish(char *EventName);