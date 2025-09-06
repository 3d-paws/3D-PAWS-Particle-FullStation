# Code Operation Notes
[←Top](../README.md)<BR>
### Timing
Every minute software takes an observation and stores it to the SD card in a dated file. Which is located under the /OBS folder.

Every second, Wind Speed & Direction occurs. If sensors for Air Particle and Distance are attached, these are also sampled.

Within the 1 second window Particle pin A1 is toggled on for 250ms. Providing the board's heartbeat.  If LoRa is activated, inbound LoRa messages are checked for multiple times.

Every 15 minutes observations are transmitted to the Particle Cloud.  The 15 minute time of transmit is based on the first occurrence of having a network connection after boot. Not having a predetermined 15 minute window, time spreads out when we transmit over the 15 minute period. This helps not overload the receiving site when large numbers of stations are reporting. 

### Time Management
A valid time source is required for normal operation and for observations to be made. There are 3 time clocks involved.
Network Time
When a network connection is made, time is obtained from the network and the System Clock is updated. A request can be made in software to perform this action. This is done every 4 hours.
#### Realtime Clock (RTC)
If the RTC is found at initialization and has a year greater equal to 2025 and less than or equal to 2035. We assume time on the RTC is valid. And we set the System Clock with this time.
If the RTC is not found or has invalid time. We must wait and get time from the network before we can make any observations.  
#### System Clock (STC)
This clock is maintained by the microcontroller. At start-up it has no valid time and must be set from the RTC or Network time. The System clock is updated every 4 hours from Network time.

Clock handling output messages during boot initialization
<div style="overflow:auto; white-space:pre; font-family: monospace; font-size: 8px; line-height: 1.5; height: 100px; border: 1px solid black; padding: 10px;">
<pre>
2000-01-01T00:00:15S - System Time
2023-10-26T18:07:24R - RTC Time
RTC:VALID            - RTC Year >= 2025 and RTC Year <= 2035
STC: Valid           - STC set from RTC and we have a valid clock source.
2025-10-26T18:07:24S - System Time
</pre>
</div>

In the main code loop when System time is valid a check. The RTC clock is serviced. If the RTC invalid time. It is set from the System time clock. Which was set from network time.  Every 2 hours we update the RTC from System time.

In the main code loop when System time is valid and we are network connected, then every 4 hours we request network time. Which results in the System clock being updated.

### Daily Reboot
A loop counter is maintained and set so around every 22 hours the system reboots itself. This is done to try and bring back online any missing sensors.  If a WatchDog board is not connected a System reset is performed.

Particle Boron Power Down when no USB power and LiPo at 10%
Random board shut offs could cause SD corruption when in the middle of writing to the SD card. To prevent this, when the Boron board has no USB power and the LiPo battery is at or below 10 percent, the board is powered off.  The board will start back up again when USB power returns.


INFO is sent at boot. When we are going to power down do to low power,  And when we request it from particle console.  So the daily reboot will cause a INFO to be sent.

### Transmitting Observations and Failure Handling
Observations are sent to Particle Cloud with Particle Event Name "FS". This is used by the Particle Cloud service to identify and associate webhooks for triggering.

After transmitting an observation, software will wait for a confirmation result code from Particle Cloud. If the result code reports failure or we didn't have network connectivity at the time of transmission. The observation is appended to the Need to Send file located on the SD card at the top level and called N2SOBS.TXT. If the file does not exist, it is created then appended to. Observations in this file will later be transmitted.

Caveat: At the time of appending the observation to the N2S file, if the file is greater than 512 * 60 * 48 bytes. ~2 days of observations. The file is deleted. Recreated and the observation is appended.

### Sending N2S Observations
After successful completion of current 1 minute observations have finished transmitting. If N2S observations exist, these are then read from the file and transmitted.  Position in the N2S file of what has been transmitted is maintained in nvram. So rebooting does not cause the retransmission of N2S observations. On a N2S observation transmit failure, N2S processing stops until the next 15 minute window.  While sending N2S observations, 1 second tasks and 1 minute observations still occur. When storage space for 1 minute observations becomes full. N2S processing stops and the main work loop takes over.  At which point current 1 minute observations will be transmitted. Freeing up the observation storage.