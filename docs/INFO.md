# Station Information - (Particle Message type "INFO")
[←Top](../README.md)<BR>
At boot the station will send a event message of type "INFO" to Particle. This message contains configuration and status information. You can also request an INFO event message to be sent from the device via DoAction Function on the Particle Console. Use keyword "INFO". Every INFO execution will create/rewrite file INFO.TXT with INFO information. The information reported will vary based on the board type (Boron, Argon, Muon). Below are examples. Check your board for what is actually presented.

INFO event message from a Boron
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 500px; border: 1px solid black; padding: 10px;">
<pre>
{
"devid":"e00fce68bde8f63590a3b118"
"devos":"6.3.3"
"freemem":19312
"uptime":10079
"type":"boron"
"mode":"FS"
"at":"2025-09-06T18:47:56"
"ver":"FS-250903v40"
"hth":64
"rr":"140-0"
"obsi":"1m"
"obsti":"15m"
"t2nt":"810s"
"drct":69771
"n2s":"NF"
"ps":"USB_HOST"
"bcs": "CHARGED"
"bpc": 100
"css":54.9996
"csq":33.3333
"imsi":""
"actsim":"INTERNAL"
"op1":"DIST 5M"
"op2":"NS"
"sensors":"TSM1(7.0),PM25AQ,RG(A3),BMX1(BMP390),MCP1,SHT1,HIH8,SI,AS5600,HI,WBT,WBGT WO/GLOB"
"lora":"NF"
"oled":"NF"
"scepin(D8)":"DISABLED"
"sce":"TRUE"
}

ps = power source
bv = battery voltage
bcs = battery charging status
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