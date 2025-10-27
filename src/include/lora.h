/*
 * ======================================================================================================================
 *  lora.h - LoRa Definations
 * ======================================================================================================================
 */
#include <RH_RF95.h>

#if (PLATFORM_ID == PLATFORM_MSOM)
/*
  #  Muon             LoRa Module
  8  MOSI to D9  SPI1 MOSI (Master Out Slave In)
  10 MISO to D10 SPI1 MISO (Master In Slave Out)
  11 SCK  to D2  SPI1 SCK
  36 CS   to D3  SPI1 ChipSel
  38 RST  to D21      Reset
  40 G0   to D20      DIO/IRQ
*/
#define LORA_IRQ_PIN  D20    // G0 on LoRa board
#define LORA_SS       D3     // Slave Select Pin aka CS
#define LORA_RESET    D21    // Used by lora_initialize()

#else

/*
  Boron            LoRa Module
  MISO to D4  SPI1 MISO
  MOSI to D3  SPI1 MOSI
  SCK  to D2  SPI1 SCK
  CS   to D9       ChipSel
  RST  to D10      Reset
  G0   to D6       DIO/IRQ
*/
#define LORA_IRQ_PIN  D6    // G0 on LoRa board
#define LORA_SS       D10   // Slave Select Pin
#define LORA_RESET    D9    // Used by lora_initialize()
#endif

#define LORA_RESET_NOACTIVITY 30 // 30 minutes

#define LORA_RELAY_MSGCNT     64  // Set to the number of LoRa Remote Messages we can buffer
#define LORA_RELAY_MSG_LENGTH 256

typedef struct {
  bool          need2log;
  int           message_type;
  char          message[LORA_RELAY_MSG_LENGTH];
} LORA_MSG_RELAY_STR;

// Extern variables
extern uint8_t  AES_KEY[16];
extern unsigned long long int AES_MYIV;
extern bool LORA_exists;
extern RH_RF95 rf95;
extern const char *relay_msgtypes[];
extern LORA_MSG_RELAY_STR lora_msg_relay[LORA_RELAY_MSGCNT];

// Function prototypes
void lora_device_initialize();
void lora_initialize();
void lora_msg_check();
void lora_msg_poll();
void lora_relay_msg_free(LORA_MSG_RELAY_STR *m);
bool lora_relay_need2log();
int lora_relay_need2log_idx();
