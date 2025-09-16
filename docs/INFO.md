# Station Information - (Particle Message type "INFO")
[←Top](../README.md)<BR>
At boot the station will send a event message of type "INFO" to Particle. This message contains configuration and status information. You can also request an INFO event message to be sent from the device via DoAction Function on the Particle Console. Use keyword "INFO". Every INFO execution will create/rewrite file INFO.TXT with INFO information. The information reported will vary based on the board type (Boron, Argon, Muon). Below are examples.

INFO event message from a Boron
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 625px; border: 1px solid black; padding: 10px;">
<pre>
{
"devid":"e00fce000006ea1d26f84df1"
"devos":"6.3.3"
"freemem":18872
"uptime":68
"type":"boron"
"mode":"FS"
"at":"2025-09-15T19:04:19"
"ver":"FS-250911v40"
"hth":0
"rr":"70-0"
"obsi":"1m"
"obsti":"15m"
"t2nt":"898s"
"drct":79199
"n2s":"NF"
"ps":"VIN"
"bcs":"CHARGED"
"bpc":93.7
"css":24.9989
"csq":37.499
"imsi":""
"actsim":"INTERNAL"
"op1":"DIST 5M"
"op2":"NS"
"sensors":"BMX1(BMP390),MCP1,SHT1,AS5600,HI,WBT,WBGT WO/GLOBE,RG(A3)"
"lora":"NF"
"oled":"NF"
"scepin(D8)":"DISABLED"
"sce":"FALSE"
}

ps = power source
bv = battery voltage
bcs = battery charging status
bpc = battery percent charge
css = cell signal strength
csq = cell signal quality
imsi = international mobile subscriber identity
obsi = observation interval (seconds)
obsti = observation transmit interval (minutes)
t2nt = time to next transmit (seconds)
drct = daily reboot countdown timer (counter)
sce = serial console enabled
scepin = status of the actual serial console pin
op1 = configuration of this pin (DIST 5M, DIST 10M, RG2, RAW, NS[Not Set])
op1 = configuration of this pin (RAW, NS[Not Set])
</pre>
</div>

INFO event message from a Argon
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 480px; border: 1px solid black; padding: 10px;">
<pre>
 {
"devid": "e00fce68a396d11b8cafc55f"
"devos":"4.1.0"
"freemem":57736
"uptime":80
"type": "argon"
"at": "2024-10-30T20:43:42"
"ver": "FSAC-241029v33"
"hth": 0
"obsi": "60s"
"obsti": "15m"
"t2nt": "236s"
"drct": "4624"
"n2s": "NF"
"ps": "USB"
"bv": 4.16972
"bcs": "!CHARGING"
"wss": 100
"wsq": 100
"mac": "e8:9f:6d:be:6d:28"
"ip": "10.0.0.1"
"mask": "255.255.255.0"
"gateway": "10.0.0.254"
"dns": "10.0.0.253"
"dhcps": "10.0.0.253"
"ssid": "SSID String"
"bssid": "cc:40:d0:87:3a:aa"
"a4": "DIST 5M"
"sensors": "BMX1(BMP390),MCP1,SHT1,HIH8,SI,AS5600,HI,WBT,WBGT WO/GLOBE"
"oled": "32"
"scepin": "DISABLED"
"sce": "TRUE"
}

wss = wireless signal strength (percent)
wsq = wireless signal  quality (percent)
bssid = mac address of the wireless gateway
</pre>
</div>

### Particle System Reset Reason (rr)

Particle Device OS provides a `resetReason` and `ReasonData` information about the cause of the last device reset on Particle Boron, Argon, Muon, and other compatible devices. This information is provided in the INFO message and on the serial console at boot. The Boron INFO message above shows this information as `"rr":"140-0"`.

---

## Overview

- `resetReason` returns a numeric **reset reason code** describing the main reason the device was last reset.
- `resetReasonData()` returns additional **reset reason data** associated with the reset, providing more specific information depending on the reset reason.

These features require enabling the `FEATURE_RESET_INFO` flag in your firmware.

---

## Reset Reason Codes

| Constant                     | Code | Supported Devices          | Description                           | Reset Reason Data               |
|------------------------------|------|---------------------------|-------------------------------------|--------------------------------|
| `RESET_REASON_NONE`           | 0    | Boron, Argon, Muon (Gen3) | No information available            | 0                              |
| `RESET_REASON_UNKNOWN`        | 10   | Yes                       | Unspecified reset reason            | None                           |
| `RESET_REASON_PIN_RESET`      | 20   | Yes                       | Reset button or reset pin triggered | None                           |
| `RESET_REASON_POWER_MANAGEMENT`| 30  | Yes                       | Low-power management reset          | None                           |
| `RESET_REASON_POWER_DOWN`     | 40   | Yes                       | Power-down reset                    | None                           |
| `RESET_REASON_POWER_BROWNOUT` | 50   | Yes                       | Brownout / power supply dip reset   | None                           |
| `RESET_REASON_WATCHDOG`       | 60   | Boron, Argon              | Hardware watchdog reset             | None                           |
| `RESET_REASON_UPDATE`         | 70   | Yes                       | Successful firmware update          | None                           |
| `RESET_REASON_UPDATE_ERROR`   | 80   | Deprecated                | Firmware update error               | None                           |
| `RESET_REASON_UPDATE_TIMEOUT` | 90   | Yes                       | Firmware update timeout             | None                           |
| `RESET_REASON_FACTORY_RESET`  | 100  | Yes                       | Factory reset requested             | 0                              |
| `RESET_REASON_SAFE_MODE`      | 110  | Yes                       | Safe mode requested                 | 0                              |
| `RESET_REASON_DFU_MODE`       | 120  | Yes                       | Device Firmware Upgrade mode        | 0                              |
| `RESET_REASON_PANIC`          | 130  | Yes                       | System panic (SOS code)             | Panic code (see below)         |
| `RESET_REASON_USER`           | 140  | Yes                       | User-requested reset                | User-defined value             |

---

## Reset Reason Data Codes (for `RESET_REASON_PANIC`)

| Code  | Meaning              |
|-------|----------------------|
| 1     | HardFault            |
| 2     | NMIFault             |
| 3     | MemManage            |
| 4     | BusFault             |
| 5     | UsageFault           |
| 6     | InvalidLength        |
| 7     | Exit                 |
| 8     | OutOfHeap            |
| 9     | SPIOverRun           |
| 10    | AssertionFailure     |
| 11    | InvalidCase          |
| 12    | PureVirtualCall      |
| 13    | StackOverflow        |
| 14    | HeapError            |
| 15    | SecureFault          |

---

### States for the Battery Charger Status (bcs)

| Code | Meaning                    |
|------|----------------------------|
|0     | BATTERY_STATE_UNKNOWN      |
|1     | BATTERY_STATE_NOT_CHARGING |
|2     | BATTERY_STATE_CHARGING     |
|3     | BATTERY_STATE_CHARGED      |
|4     | BATTERY_STATE_DISCHARGING  |
|5     | BATTERY_STATE_FAULT        |
|6     | BATTERY_STATE_DISCONNECTED |

---
