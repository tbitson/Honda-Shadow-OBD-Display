# Honda Shadow OBD Display

This is a 'in-work'n project. Many aspects may not work yet!
 
A Teensy 3.2 based OBD display for a 
2013 Honda Shadow 750 Aero. 

There are 2 parts to this project:
1) A level converter/interface board that connects to the diagnostic connector on the motorcycle. This provides power and opto-isolates the OBD signal. The OBD signal is level shifted to a 3.3V logic level. The schmatic and Eagle PCB files are included.

2) A Teensy 3.2 microcontroller connected to a 240x320 LCD Touchscreen. This displays the various options and OBD values. GThe Teensy and LCD are from PRJC.com. It uses the PCB LCD adapter from PRJC to connect the LCD to the Teensy, and is available from the OSHPark.com.


