# LoRa Remote Relay
[←Top](../README.md)<BR>
When LoRa is enabled on the station by adding the LoRa Module and configuring CONFIG.TXT on the SD card. LoRa messages from remote LoRa stations are received and stored in memory. Up to 64 messages. At the next transmit windows these observations are sent to Particle. They will be added to the N2S file when necessary.

For information on LoRa Remote stations see [3D-PAWS-Feather-LoRaRemoteM0](https://github.com/3d-paws/3D-PAWS-Feather-LoRa-Remote) code base.

LoRa message type received for relay are:

- "INFO" : Sent at boot providing station configuration information
- "LR" : LoRa Relay message containing observation information.

Tibits of Information:
- LoRa messages are AES 128 bit encrypted.
- LoRa messages are in JSON format
- The Remote Lora device messages will include a device id (devid) and a CHORDS station id (id). 

---

## SD Configuration file CONFIG.TXT
The aes_pkey and aes_myiv must match the Remote Lora device configuration
<pre>
#
# CONFIG.TXT
#
# Line Length is limited to 63 characters
#12345678901234567890123456789012345678901234567890123456789012

# Private Key - 128 bits (16 bytes of ASCII characters)
aes_pkey=10FE2D3C4B5A6978

# Initialization Vector must be and always will be 128 bits (16 bytes.)
# The real iv is actually myiv repeated twice
# 1234567 -> 0x12D687 = 0x00 0x12 0xD6 0x87 0x00 0x12 0xD6 0x87 
aes_myiv=1234567

# This unit's LoRa ID for Receiving and Sending Messages
lora_unitid=1

# You can set transmitter power from 5 to 23 dBm
lora_txpower=5

# Valid entries are 433, 866, 915
lora_freq=915
</pre>

