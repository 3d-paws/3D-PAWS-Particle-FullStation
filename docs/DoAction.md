# Particle Cloud Console DoAction Functions
[←](../README.md)<BR>

On the view device screen on the Particle Console there is a FUNCTIONS area at the lower right. This is used to send commands to the online device.

* REBOOT - Reboot device. Toggle pin A0. If no relay/watchdog is connected to A0, then a soft boot on the Particle board is performed.
* SEND - Send queued observations.
* INFO - Trigger station to send station information. Event type "INFO".
* CRT - Clear rain totals that are stored in nvram.
* TXI5M - Set 1 Minute Observations, Set Transmit Interval to 5 minutes. Creates file TXI5M.TXT. Removes TXI10M.TXT file.
* TXI10M - Set 1 Minute Observations, Set Transmit Interval to 10 minutes. Creates file TXI10M.TXT. Removes TXI5M.TXT file.
* TXI15M - Set 1 Minute Observations, Set Transmit Interval to 15 minutes (Default). Removes TXI5M.TXT and TXI10M.TXT files.
* OBI5M - Set 5 Minute Observations, Transmit Interval to 5 Minutes (OBI5M.TXT)
* OBI10M - Set 10 Minute Observations, Transmit Interval to 10 Minutes (OBI10M.TXT)
* OBI15M - Set 15 Minute Observations, Transmit Interval to 15 Minutes (OBI15M.TXT)

Argron and Boron Pins OP1=A4, OP2=A5 and Muon Pins OP1=A0, OP2=A2
* OP1DIST - Configures pin for a distance gauge (OP1DIST.TXT)
* OP1D5M - Configures 5m Sensor. Value read from pin is multiplied by 1.25mm (OP1D5M.TXT)
* OP1RAIN - Configures pin for 2nd rain gauge (OP1RAIN.TXT)
* OP1RAW - Configures pin for raw analog readings (OP1RAW.TXT)
* OP1CLR - Unassigns pin. Removes associated TXT files.
* OP2RAW - Configure for raw readings. Average of 5 samples 10ms apart (OP2RAW.TXT)
* OP2CLR - Unassigns pin. Removes associated TXT files.