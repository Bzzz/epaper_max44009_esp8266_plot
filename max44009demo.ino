#include <GxEPD.h>
#include <GxGDEW042Z15/GxGDEW042Z15.cpp>    // 4.2" b/w/r
#include GxEPD_BitmapExamples
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

#if defined(ESP8266)
// generic/common.h
//static const uint8_t SS    = 15;
//static const uint8_t MOSI  = 13;
//static const uint8_t MISO  = 12;
//static const uint8_t SCK   = 14;
// pins_arduino.h
//static const uint8_t D8   = 15;
//static const uint8_t D7   = 13;
//static const uint8_t D6   = 12;
//static const uint8_t D5   = 14;
// GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
GxIO_Class io(SPI, SS, 0, 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
// GxGDEP015OC1(GxIO& io, uint8_t rst = 2, uint8_t busy = 4);
GxEPD_Class display(io); // default selection of D4(=2), D2(=4)
#elif defined(ESP32)
// pins_arduino.h, e.g. LOLIN32
//static const uint8_t SS    = 5;
//static const uint8_t MOSI  = 23;
//static const uint8_t MISO  = 19;
//static const uint8_t SCK   = 18;
// GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
GxIO_Class io(SPI, SS, 17, 16); // arbitrary selection of 17, 16
// GxGDEP015OC1(GxIO& io, uint8_t rst = D4, uint8_t busy = D2);
GxEPD_Class display(io, 16, 4); // arbitrary selection of (16), 4
#else
// pins_arduino.h, e.g. AVR
//#define PIN_SPI_SS    (10)
//#define PIN_SPI_MOSI  (11)
//#define PIN_SPI_MISO  (12)
//#define PIN_SPI_SCK   (13)
GxIO_Class io(SPI, SS, 8, 9); // arbitrary selection of 8, 9 selected for default of GxEPD_Class
GxEPD_Class display(io);
#endif

 
//ü is 0xfc
//ä is 0xe4
//ö is 0xf6
//Ä is 0xc4
//Ö is 0xd6
//Ü is 0xdc
//print(char(0x81))

#include "Wire.h"
#include "Max44009.h"
Max44009 myLux(0x4A, D6, D1);  // 0x4a; scl = D1, sda = D6
#define drawcolor GxEPD_RED
float lux = 0;
int err = 0;
int count;
int countx;
int sensorArray[400];
int drawHeight;
int disph = 299;
int dispw = 399;
int dispsteph = 30;   //pixels
int dispstepw = 40;   //pixels
int readings = 5;
int rcount = 5;      //draw on first loop
char filled = 'F'; //decide either filled or dot display (D=dot, any else filled)
char drawDirection = 'R'; //decide drawing direction, from right or from left (L=from left to right, any else from right to left)
char slope = 'S'; //slope colour of filled mode white or black slope (S=dot color, any else black)
int mapmax = 10;
float mapfactor = 1;
int maptemp = 0;
void setup() {
  Serial.begin(115200);

  display.init();

  display.fillScreen(GxEPD_WHITE);
  display.update();
  Max44009 myLux(0x4A, D6, D1);  // for some reason, single init doesn't cut it, the display init overwrites stuff
  for (count = 0; count <= dispw + 1; count++) //zero all elements
  {
    sensorArray[count] = 0;
  }
}

void loop() {
  lux = myLux.getLux();   err = myLux.getError();
  if (err != 0)   {
    Serial.print("Error:\t");
    Serial.println(err);
  }   else   {
    Serial.print("lux:\t");
    Serial.println(lux);
  }
  if (lux > mapmax) {
    maptemp = mapmax;
    while (maptemp >= 10){
      maptemp /= 10;
    }
    if (maptemp == 2) {
      mapfactor = 2.5;  //crude autoscale
    } else {
      mapfactor = 2;
    }
    mapmax = mapfactor * mapmax;
    Serial.print("Autoscale -> "); Serial.println(mapmax);
    rcount = readings;    //force redraw
  }
  sensorArray[0] = lux;   //write raw value into array - scale later
  for (count = 1; count <= dispw + 1; count++ )   {
    if (filled == 'D' || filled == 'd')     {
      if (drawDirection == 'L' || drawDirection == 'l')       {
        display.drawPixel(count, disph + 1 - map(sensorArray[count - 1], 0, mapmax, 0, disph + 1 ), drawcolor);
      }       else //else, draw dots from right to left
      {
        display.drawPixel(dispw + 1 - count, disph + 1 - map(sensorArray[count - 1], 0, mapmax, 0, disph + 1 ), drawcolor);
      }
    }     else     {
      if (drawDirection == 'L' || drawDirection == 'l')       {
        if (slope == 'S' || slope == 'S')         {
          display.drawLine(count, dispw + 1, count, disph + 1 - map(sensorArray[count - 1], 0, mapmax, 0, disph + 1 ), drawcolor);
        }         else         {
          display.drawLine(count, dispw + 1, count, disph + 1 - map(sensorArray[count - 1], 0, mapmax, 0, disph + 1 ), GxEPD_BLACK);

        }
      }       else       {
        if (slope == 'S' || slope == 'S')         {
          display.drawLine(dispw + 1 - count, disph, dispw + 1 - count, disph + 1 - map(sensorArray[count - 1], 0, mapmax, 0, disph + 1 ), drawcolor);
        }         else         {
          display.drawLine(dispw + 1 - count, disph, dispw + 1 - count, disph + 1 - map(sensorArray[count - 1], 0, mapmax, 0, disph + 1 ), GxEPD_BLACK);
       }
      }
    }
  }

 
  drawAxises();   rcount++;
  if (rcount >= readings) {
    display.update();  //needed before anything is displayed
    rcount = 0;
  }
  display.fillScreen(GxEPD_WHITE); //clear before new drawing

  for (count = dispw + 1; count >= 2; count--) // move array
  {
    sensorArray[count - 1] = sensorArray[count - 2];
  }

 
  delay(1000);
}

 
void drawAxises()  //separate to keep stuff neater. This controls ONLY drawing background!
{
  display.setCursor(320, 20);   display.setFont(&FreeSansBold9pt7b);   display.setTextColor(GxEPD_BLACK);   display.print(lux);   //unscaled raw value
  display.setCursor(320, 40);   display.print("lux");

 
  display.drawLine(0, 0, 0, disph, GxEPD_BLACK);   display.drawLine(dispw, 0, dispw, disph, GxEPD_BLACK);

  display.drawLine(0, 0, dispw, 0, GxEPD_BLACK);   display.drawLine(0, disph, dispw, disph, GxEPD_BLACK);

  for (count = dispsteph; count < disph + 1; count += dispsteph)   {
    display.drawLine(dispw - 5, count, dispw, count, GxEPD_BLACK);  //ticks left/right
    display.drawLine(0, count, 5, count, GxEPD_BLACK);

    for (countx = 0; countx < dispw + 1; countx += dispstepw)     {
      display.drawPixel(countx, count, GxEPD_BLACK);        //dots center
    }
    display.setCursor(0.25 * dispstepw, disph + 1 - count + 0.33 * dispsteph);     display.print(map(count, 0, disph + 1, 0, mapmax ));
  }

  for (count = 0; count < dispw + 1; count += dispstepw)  {
    display.drawLine(count, 0, count, 5, GxEPD_BLACK);        //ticks top/bottom
    display.drawLine(count, disph - 5, count, disph, GxEPD_BLACK);
  }
}
