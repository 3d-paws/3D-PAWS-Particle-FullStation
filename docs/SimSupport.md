# 3rd Part Sim Support
[←](Boron.md)<BR>
On the underside of the Particle Boron board is a slot for a SIM card. This SIM slot can be used instead of the built-in SIM. Particle's software stores the state of what SIM is being used and cell provider information in nvram. The state is either set INTERNAL or EXTERNAL. We can control this state by the existence of the SD file SIM.TXT.  The user needs to create file SIM.TXT and place it at the top directory on the SD card.

If the SIM.TXT file contains the below and it passes syntax checks. Then at boot software sets the SIM to use to EXTERNAL and the file is then renamed to SIMOLD.TXT, so we don't do this on the next boot. Below shows options for the SIM.TXT. What is used will depend on what is needed by the SIM. Select one of the 3 options below.

Option 1: Connect to a cellular network using APN, USERNAME and PASSWORD

>AUP epc.tmobile.com username passwd

Option 2: Connect to a cellular network using USERNAME and PASSWORD

>UP username password

Option 3: Connect to a cellular network using only APN

>APN apn

Below shows an example of what was needed for the T-Mobile SIM.

>APN epc.tmobile.com

If the SIM.TXT file contains the below and it passes syntax checks. Then at boot software sets the SIM to use to INTERNAL and the file is then renamed to SIMOLD.TXT, so we don't do this on the next boot.

>INTERNAL

If you have the serial console enabled, you will see messages related to the above actions. Upon SIM setup completion, the board led will flash forever. Indicating a reboot is needed.

SEE: <https://docs.particle.io/troubleshooting/guides/connectivity-troubleshooting/using-3rd-party-sim-cards/>