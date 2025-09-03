# Argon WiFi Board Support
[←](../README.md)<BR>

When setting up an Argon for the first time you use the Particle phone app and pair with the Argon board. This process includes configuring the WiFi settings.  These settings are saved in nonvolatile memory.  At this point you are done. But, if you want to change WiFi settings stored in nonvolatile memory you can: Use the app on your phone or create a file on the SD card with the WiFi credentials.

WiFi credentials file  
At the top level of the SD card, create a file called WIFI.TXT. This file has one line with 3 comma separated items. Example:

>AuthType,ssid,password

Where AuthType is one of these keywords  
>UNSEC  
WEP  
WPA  
WPA2  
WPA_ENTERPRISE  
WPA2_ENTERPRISE  

When WiFi Information is successfully read and parsed from the file at startup, the below will display on Serial Console:
WIFI:Open
WIFI:ATYPE[WPA2]
WIFI:SSID[NotMySSID]
WIFI:PW[NotMyPassword]
WIFI:Credentials Cleared
WIFI:Credentials Set [One of the above AuthTypes]

If WiFi file not found the below will display on Serial Console:  
>WIFI:NOFILE USING NVAUTH

Note: If something went wrong obtaining WiFi credentials the credentials in non volatile memory are used.