#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>
#include <TFT.h>
#include <EEPROM.h>

#define SD_CS 4
#define TFT_CS 10
#define DC 9
#define RST 8

#define SPEAKER 6
#define BUTTON_A 2
#define BUTTON_B 3
#define BUTTON_C 5

#define INPUT_V A1 /*via 10K - 1K voltage divider at input V after fuse*/
#define INPUT_A A0 /*analog out from ACS712 module*/

/* ------------ items --------- */
#define NONE  -1
#define LOG   0
#define MAX_W 1
#define MAX_A 2
#define MAX_V 3
#define MIN_V 4
#define DATE  5
#define TIME  6
#define ITEMS 7 /* amount of different items */

RTC_DS1307 rtc;
TFT tft = TFT(TFT_CS, DC, RST);

boolean updateMinV = true;
boolean updateMaxV = true;
boolean updateMaxA = true;
boolean updateMaxW = true;
boolean updateDate = true;
boolean updateTime = true;
boolean updateLog = true;

int item = LOG;
int pItem = NONE;
int selectedItem = NONE;
int pSelectedItem = NONE;

boolean buttonA = true;
boolean buttonB = true;
boolean buttonC = true;
boolean pButtonA = true;
boolean pButtonB = true;
boolean pButtonC = true;

uint16_t white;
uint16_t grey;
uint16_t black;
uint16_t red;
uint16_t green;
uint16_t lightGreen;

float pVolt = 9999;
float volt = 0;
int yVolt = 1;
int minVolt = 30;
int maxVolt = 45;

float pAmpere = 9999;
float ampere = 0;
int yAmpere = 39;
int minAmpere = 0;
int maxAmpere = 10;

float pWatt = 9999;
float watt = 0;
int yWatt = 77;
int minWatt = 0;
int maxWatt = 300;

char timeString[9] = "00:00:00";
char dateString[9] = "00/00/00";
char pTimeString[9];
char pDateString[9];
int newYear;
int newMonth;
int newDay;
int newHour;
int newMinute;
int newSecond;

int loopCount = 0;
uint8_t graphCursor = 0;

boolean sdFail = false;
unsigned int fileNumber = 0;
char fileName[13];

unsigned long nextLoop = 0; // millis when next loop starts
unsigned long logStart = 0;




//=================================================================================== SETUP
void setup() {


  // ----------------------------------------------------- read eeprom
   int addr = 0;
   // EEPROM.get(addr, minVolt); addr += sizeof(int);
   // EEPROM.get(addr, maxVolt); addr += sizeof(int);
   // EEPROM.get(addr, maxAmpere); addr += sizeof(int);
   // EEPROM.get(addr, maxWatt); addr += sizeof(int);
    EEPROM.get(addr, fileNumber);
  


  // ----------------------------------------------------- serial
  Serial.begin(9600);
  // ----------------------------------------------------- rtc init
  rtc.begin();
  strcpy(pTimeString, timeString);
  strcpy(pDateString, dateString);
  // ----------------------------------------------------- tft
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  drawScreenLayout();
  nextLoop = millis();
}

//=================================================================================== LOOP
void loop() {
  //----------------------------- loop count
  loopCount++;

  //----------------------------- timing
  while (millis() < nextLoop);
  nextLoop = max(nextLoop + 100, millis() + 100); // aim for 10 Hz refresh rate

  //----------------------------- rtc
  checkTime();
  if (strcmp(dateString, pDateString))updateDate = true;
  if (strcmp(timeString, pTimeString))updateTime = true;
  strcpy(pTimeString, timeString);
  strcpy(pDateString, dateString);


  //----------------------------- buttons (switching between items)

  checkButtons(); // check buttons and do button / menu / item logic

  //----------------------------- measure analog inputs
  float fm = millis();



  volt   = readVolt();
  ampere = readAmpere();
  watt   = constrain(volt * ampere, 0, 999);

  //----------------------------- lo voltage
  if (volt < 6) {

    tftClear();
    tft.setTextSize(2);
    tft.setTextColor(white);
    tft.setCursor(25, 55);
    tft.print("LOW POWER");

    while (readVolt() < 6.2) { // wait until voltage is higher again
      delay(20);
    };

    tftClear();
    drawScreenLayout();
    updateMinV = true;
    updateMaxV = true;
    updateMaxA = true;
    updateMaxW = true;
    updateDate = true;
    updateTime = true;
    updateLog  = true;
    pVolt=9999;
    pAmpere=9999;
    pWatt=9999;

  } else {


    //----------------------------- sd logging
    if (selectedItem == LOG) {
      sdLog();
    }
    if (sdFail && selectedItem == LOG) {
      selectedItem = NONE;
      updateLog = true;
      errorSound();
    }

    //----------------------------- draw items (only if the have to be updated)
    drawMinVolt(MIN_V, updateMinV);
    drawMaxVolt(MAX_V, updateMaxV);
    drawMaxAmpere(MAX_A, updateMaxA);
    drawMaxWatt(MAX_W, updateMaxW);

    drawDate(DATE, updateDate);
    drawTime(TIME, updateTime);
    drawLogging(LOG, updateLog);

    updateMinV = false;
    updateMaxV = false;
    updateMaxA = false;
    updateMaxW = false;
    updateDate = false;
    updateTime = false;
    updateLog = false;

    //----------------------------- draw big numbers (only new digits will be drawn)
    drawBigNumber(volt, pVolt, 82,  yVolt + 12);
    drawBigNumber(ampere, pAmpere, 82,  yAmpere + 12);
    drawBigNumber(watt, pWatt, 82,  yWatt + 12);
    //----------------------------- draw graphs
    graphCursor++;
    graphCursor = graphCursor % 77;
    tft.drawFastVLine((graphCursor + 1) % 77 + 1, 2, 113, grey); // cursor
    tft.drawFastVLine(graphCursor + 1, 2, 113, black); // clear bgnd
    tft.stroke(white);
    if (graphCursor % 2) {
      tft.point(graphCursor + 1, yAmpere); // horizontal lines
      tft.point(graphCursor + 1, yWatt);
    }

    int graphV = constrain(map(volt * 10, minVolt * 10, maxVolt * 10, 37, 1), 1, 37);
    int graphA = constrain(map(ampere * 10, minAmpere * 10, maxAmpere * 10, 37, 1), 1, 37);
    int graphW = constrain(map(watt * 10, minWatt * 10, maxWatt * 10, 37, 1), 1, 37);

    int pGraphV = constrain(map(pVolt * 10, minVolt * 10, maxVolt * 10, 37, 1), 1, 37);
    int pGraphA = constrain(map(pAmpere * 10, minAmpere * 10, maxAmpere * 10, 37, 1), 1, 37);
    int pGraphW = constrain(map(pWatt * 10, minWatt * 10, maxWatt * 10, 37, 1), 1, 37);


    pVolt = volt;
    pAmpere = ampere;
    pWatt = watt;


    tft.stroke(white);
    tft.line(graphCursor + 1, graphV + yVolt,   graphCursor , pGraphV + yVolt);     // graph V
    tft.line(graphCursor + 1, graphA + yAmpere, graphCursor , pGraphA + yAmpere);   // graph A
    tft.line(graphCursor + 1, graphW + yWatt,   graphCursor , pGraphW + yWatt);     // graph W


    tft.line(graphCursor + 1, graphV + yVolt + 1,   graphCursor , pGraphV + yVolt + 1); // graph V
    tft.line(graphCursor + 1, graphA + yAmpere + 1, graphCursor , pGraphA + yAmpere + 1); // graph A
    tft.line(graphCursor + 1, graphW + yWatt + 1,   graphCursor , pGraphW + yWatt + 1); // graph W
  }

  //----------------------------- fixing gfx bug
  tft.stroke(255, 255, 255);
  tft.line(159, 1, 159, 116);
}


//----------------------------- read volt
float readVolt() {
  int vIn = 0;
  for (int i = 0; i < 10; i++) {
    vIn += analogRead(INPUT_V);
  }
  return float(vIn) * 11 / 1023 * 5 * .989 / 10;
}

//----------------------------- read ampere
float readAmpere() {
  int aIn = 0;
  for (int i = 0; i < 10; i++) {
    aIn += analogRead(INPUT_A);
  }
  return float(abs(aIn - 5100)) * 5000 / 1023 / 66 / 10; // ACS712 delivers 185mV/A on Pin A0 :
}
