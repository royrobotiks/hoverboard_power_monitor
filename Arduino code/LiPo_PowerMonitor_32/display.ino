//---------------------------------------------------------- redraw items? (redraw if selection has changed and set colors for bgnd)
boolean redrawItem(int itemIndex) {
  boolean returnValue = false;
  uint16_t bgnd = black;
  if (item == itemIndex) {
    bgnd = red;
    if (pItem != itemIndex) returnValue = true;
  } else {
    if (pItem == itemIndex) returnValue = true;
  }
  if (selectedItem == itemIndex) {
    bgnd = green;
    if (pSelectedItem != itemIndex) returnValue = true;
  } else {
    if (pSelectedItem == itemIndex) returnValue = true;
  }
  tft.fill(bgnd); // fill background color
  tft.stroke(bgnd);
  return returnValue;
}

//---------------------------------------------------------- items:
//.......................................................... min volt
void drawMinVolt(int me, boolean forcedUpdate) {
  if (redrawItem(me) || forcedUpdate) {
    tft.rect(101, yVolt + 2, 15, 9);
    tft.setTextSize(1);
    tft.setTextColor(white);
    tft.setCursor(103, yVolt + 3);
    tft.print(minVolt);
  }
}
//.......................................................... max volt
void drawMaxVolt(int me, boolean forcedUpdate) {
  if (redrawItem(me) || forcedUpdate) {
    tft.rect(140, yVolt + 2, 15, 9);
    tft.setTextSize(1);
    tft.setTextColor(white);
    tft.setCursor(142, yVolt + 3);
    tft.print(maxVolt);
  }
}
//.......................................................... min ampere
void drawMinAmpere() {
  tft.setTextSize(1);
  tft.setTextColor(white);
  tft.setCursor(103, yAmpere + 3);
  tft.print(minAmpere);
}
//.......................................................... max ampere
void drawMaxAmpere(int me, boolean forcedUpdate) {
  if (redrawItem(me) || forcedUpdate) {
    tft.rect(140, yAmpere + 2, 15, 9);
    tft.setTextSize(1);
    tft.setTextColor(white);
    tft.setCursor(142, yAmpere + 3);
    tft.print(maxAmpere);
  }
}
//.......................................................... min watt
void drawMinWatt() {
  tft.setTextSize(1);
  tft.setTextColor(white);
  tft.setCursor(103, yWatt + 3);
  tft.print(minWatt);
}
//.......................................................... max watt
void drawMaxWatt(int me, boolean forcedUpdate) {
  if (redrawItem(me) || forcedUpdate) {
    tft.rect(134, yWatt + 2, 21, 9);
    tft.setTextSize(1);
    tft.setTextColor(white);
    tft.setCursor(136, yWatt + 3);
    tft.print(maxWatt);
  }
}
//.......................................................... date
void drawDate(int me, boolean forcedUpdate) {
  if (redrawItem(me) || forcedUpdate) {
    tft.rect(0, 118, 51, 10);
    tft.setTextSize(1);
    tft.setTextColor(white);
    tft.setCursor(2, 119);
    tft.print(dateString);
  }
}
//.......................................................... time
void drawTime(int me, boolean forcedUpdate) {
  if (redrawItem(me) || forcedUpdate) {
    tft.rect(57, 118, 51, 10);
    tft.setTextSize(1);
    tft.setTextColor(white);
    tft.setCursor(59, 119);
    tft.print(timeString);
  }
}
//.......................................................... logging
void drawLogging(int me, boolean forcedUpdate) {
  if (redrawItem(me) || forcedUpdate) {

    if (sdFail && item == me) { // draw bgnd
      tft.fill(red);
      tft.stroke(red);
    }
    tft.rect(115, 118, 45, 10);
    tft.setTextSize(1);

    tft.setTextColor(white); // draw text
    tft.setCursor(117, 119);

    if (sdFail) {
      tft.print("SD FAIL");
    } else {
      if (selectedItem == LOG) {
        tft.print("LOGGING");
      } else {
        tft.print("-> LOG?");
      }
    }
  }
}


//---------------------------------------------------------- draws a big number on the screen - only draws new characters compared to 'pNumber'
void drawBigNumber(float inputFloat, float pInputFloat, int xPos, int yPos) {

  // numbers to strings

  char number[5];
  if (inputFloat >= 100) {
    sprintf(number, "%04d", int(inputFloat));
  } else {
    sprintf(number, "%02d.%01d", int(inputFloat), int(inputFloat * 10) - int(inputFloat) * 10);
  } /*else if (inputFloat >= 1) {
    sprintf(number, "%01d.%02d", int(inputFloat), int(inputFloat * 100) - int(inputFloat) * 100);
  } else if (inputFloat < 1) {
    sprintf(number, ".%03d", int(inputFloat * 1000));
  }*/

  char pNumber[5];
  if (pInputFloat >= 100) {
    sprintf(pNumber, "%04d", int(pInputFloat));
  } else  {
    sprintf(pNumber, "%02d.%01d", int(pInputFloat), int(pInputFloat * 10) - int(pInputFloat) * 10);
  }/* else if (pInputFloat >= 1) {
    sprintf(pNumber, "%01d.%02d", int(pInputFloat), int(pInputFloat * 100) - int(pInputFloat) * 100);
  } else if (pInputFloat < 1) {
    sprintf(pNumber, ".%03d", int(pInputFloat * 1000));
  }*/

  tft.setTextSize(3);
  tft.setTextColor(white);
  int spacing = 2;
  for (int i = 0; i < 4; i++) {                    //iterate through digits of number string
    char newDigit = number[i];               // compare old and new digits - only draw new ones
    char oldDigit = pNumber[i];
    if (newDigit != oldDigit || loopCount == 1) {
      tft.fillRect(xPos + i * 19, yPos, 17, 24, black);      // background
      tft.setCursor(xPos + 1 + i * 19, yPos + 1);
      tft.print(newDigit);
    }
  }
}

// ======================================================= convert rgb to gfx-library-style 2-byte color value

uint16_t rgb(float r, float g, float b) {
  r = r * 31;
  g = g * 63;
  b = b * 31;

  uint8_t ir = constrain(r, 0, 31);
  uint8_t ig = constrain(g, 0, 63);
  uint8_t ib = constrain(b, 0, 31);

  uint16_t returnColor = (ir & 0b00011111) << 11 | (ig & 0b00111111) << 5 | (ib & 0b00011111);
  return returnColor;
}

// =============================================clear
void tftClear() {
  tft.background(0, 0, 0);
  //tft.fillRect(0, 0, 161, 128, rgb(0, 0, 0));           // blank screen
}

// ===================================================layout
void  drawScreenLayout(){
    tftClear();
  white = rgb(1, 1, 1);
  black = 0;
  red = rgb(1, 0, 0);
  green = rgb(0, .7, .3);
  grey = rgb(.5, .5, .5);
  lightGreen = rgb(0, 1, .5);

  // ----------------------------------------------------- draw value boxes
  tft.fill(0,0,0);
  tft.stroke(255, 255, 255);
  tft.rect(78, yVolt, 82, 39);
  tft.rect(78, yAmpere, 82, 39);
  tft.rect(78, yWatt, 82, 39);

  // ----------------------------------------------------- draw graph boxes
  tft.stroke(255, 255, 255);
  tft.fill(0, 0, 0);
  tft.rect(0, yVolt, 79, 39);
  tft.rect(0, yAmpere, 79, 39);
  tft.rect(0, yWatt, 79, 39);


  //----------------------------- draw legend and scale
  tft.setTextSize(1);
  tft.setTextColor(white);
  tft.setCursor(83, yVolt + 3);
  tft.print("V:    ...");
  tft.setCursor(83,  yAmpere + 3);
  tft.print("A:    ...");
  tft.setCursor(83,  yWatt + 3);
  tft.print("W:   ...");
  drawMinAmpere();
  drawMinWatt();
  }
