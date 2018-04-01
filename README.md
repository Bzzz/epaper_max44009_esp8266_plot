# epaper_max44009_esp8266_plot
Demo plot code for the Waveshare 4.2" 400x300 ePaper module paired with the Wemos D1 ESP8266 board and a GY-49 MAX44009 ambient light sensor


This requires the following additions to your Arduino installation:
* ESP8266 support (if not already shipped with your Arduino board packages, use the library manager to download something appropriate, e.g. Blynk, generic ESP libraries)
* the Adafruit GFX library (library manager, tested with v1.2.3)
* Waveshare ePaper library from https://github.com/ZinggJM/GxEPD 
* MAX44009 library from https://github.com/RobTillaart/Arduino (thanks for enhancing your code to support ESP8266 devices)


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

This demo is based on the code from some unknown guy over at https://justpaste.it/16cp0 - heavily modified, but working the same due to using the same Adafruit GFX base code. It should be easy to port this back for SSD1306 based displays with different resolution, as I have removed all hardcoded sizes from the main loop and the axis plot. Don't blame me for the extensive use of IF statements, I didn't respin the entire thing.
