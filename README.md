# epaper_max44009_esp8266_plot
Demo plot code for the Waveshare 4.2" 400x300 ePaper module paired with the Wemos D1 ESP8266 board and a GY-49 MAX44009 ambient light sensor


This requires the following additions to your Arduino installation:
* ESP8266 support (if not already shipped with your Arduino board packages, use the library manager to download something appropriate, e.g. Blynk, generic ESP libraries)
* the Adafruit GFX library (library manager, tested with v1.2.3)
* Waveshare ePaper library from https://github.com/ZinggJM/GxEPD
* MAX44009 library from https://github.com/RobTillaart/Arduino


Wiring as per ZinggJM's recommendation:

BUSY -> D2

RST -> D4

DC -> D3

CS -> D8

CLK -> D5

DIN -> D7

GND -> GND (shared with MAX44009 board)

3.3V -> 3.3V (shared with MAX44009 board)

SCL -> D1

SDA -> D6



Avoid D0 - do NOT use 5V on any of the board attachments, they are 3V3 only!

Other than that, this demo code should compile, upload and run just fine.
 
