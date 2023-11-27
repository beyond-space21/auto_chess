#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>

#include "colors.h"

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// TouchScreen ts = TouchScreen(XP, YP, XM, YM, 100);

void setup(){
    Serial.begin(9600);

    // Reset display
    tft.reset(); 
    // If your display does not work, change the address to 0x9325 or 0x9328 or 0x7575 or 0x9341, 0x8357
    tft.begin(0x9341); 
    tft.setRotation(3);
    tft.fillScreen(red);
    // tft.drawPixel(0,0,white);
    // tft.drawPixel(1,1,lime);
    // tft.drawPixel(2,2,white);
    // tft.drawPixel(3,3,white);
    // tft.drawPixel(4,4,white);
    // tft.drawPixel(5,5,white);
    // tft.drawPixel(6,6,white);
}

void loop(){
    // TSPoint p = ts.getPoint();

    // pinMode(XM, OUTPUT);
    // pinMode(YP, OUTPUT);

    // if(p.z > ts.pressureThreshhold){
    //     int y = map(p.x, TS_MAXX, TS_MINX, 0, 320);
    //     int x = map(p.y, TS_MAXY, TS_MINY, 0, 480);

    //     if(winnerWasDrawn == true){
    //         if(10 < x && x < 10 + 140 && 260 < y && y < 260 + 50){
    //             Serial.println("Reseting...");
    //             resetFunc();
    //         }
    //     }

    //     for(int _y = 0; _y < 3; _y++){
    //         for(int _x = 0; _x < 3; _x++){
    //             int xPos = xOffset + padding + nextPos * _x;
    //             int yPos = padding + nextPos * _y;

    //             if(xPos < x && x < xPos + nextPos && yPos < y && y < yPos + nextPos){
    //                 move(_x, _y);
    //                 break;
    //             }
    //         }
    //     }
    // }
}