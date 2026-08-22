/*
 * ======================================================================================================================
 *  ps.h - Particle Support Definations
 * ======================================================================================================================
 */

enum OTAState {
    NO_UPDATE,
    UPDATE_PENDING,
    WAITING_FOR_UPDATE,
    UPDATE_FAILED
};

// Extern variables
#if (PLATFORM_ID == PLATFORM_BORON) || (PLATFORM_ID == PLATFORM_MSOM)
extern char imsi[16];
#endif

extern const char *batterystate[];

extern volatile OTAState ota_state;
extern unsigned long ota_stateStarted;
extern const unsigned long UPDATE_WAIT_MS;

// Function prototype
void enterOTAState(OTAState s);
bool getCellularGlobalIdentity(char *buf, size_t buf_len);
void GetPinName(pin_t pin, char *pinname);
void OutputResetReason();
void Output_CellBatteryInfo();
void DeviceReset();
int Function_DoAction(String s);

#if PLATFORM_ID == PLATFORM_ARGON
void ARGON_network_initialize();
#endif

#if PLATFORM_ID == PLATFORM_MSOM
void MUON_network_initialize();
#endif

#if (PLATFORM_ID == PLATFORM_ARGON) || (PLATFORM_ID == PLATFORM_MSOM)
void WiFiPrintCredentials();
#else
void SimChangeCheck();
#endif

#if (PLATFORM_ID == PLATFORM_BORON) || (PLATFORM_ID == PLATFORM_MSOM)
int callback_imsi(int type, const char* buf, int len, char* cimi);
#endif
