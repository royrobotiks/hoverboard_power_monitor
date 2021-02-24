unsigned long pushTimeA;
unsigned long longPushA;
unsigned long pushTimeB;
unsigned long longPushB;
unsigned long pushTimeC;
int pushRepeatA = 0; // keeps track of auto pushes when button is pushed for a long time
int pushRepeatB = 0;

void checkButtons() {
  pItem = item;
  pSelectedItem = selectedItem;

  pButtonA = buttonA;
  pButtonB = buttonB;
  pButtonC = buttonC;
  buttonA = digitalRead(BUTTON_A);
  buttonB = digitalRead(BUTTON_B);
  buttonC = digitalRead(BUTTON_C);

  if (!buttonA) {
    buttonB = 1;  // only one button can be regarded as pushed
    buttonC = 1;
  }
  if (!buttonB) {
    buttonA = 1;
    buttonC = 1;
  }
  if (!buttonC) {
    buttonA = 1;
    buttonB = 1;
  }

  boolean aPushed = (!buttonA && pButtonA); // button a is pushed
  boolean bPushed = (!buttonB && pButtonB); // button b is pushed
  boolean cPushed = (!buttonC && pButtonC); // button c is pushed

  if (aPushed)pushTimeA = millis();
  if (bPushed)pushTimeB = millis();

  // ............................................................................. repeated pushs / a&b button
  if (buttonA) {
    longPushA = 700; // default value for waiting until repeated pushs happen
    pushRepeatA = 0;
  }
  if (!buttonA && pushTimeA + longPushA < millis()) {
    aPushed = true;
    pushRepeatA++;
    if (pushRepeatA < 5) {
      longPushA += 200; // repetition time of repeated pushs
    } else {
      longPushA += 100; // repetition time of repeated pushs
    }
  }
  if (buttonB) {
    longPushB = 700; // default value for waiting until repeated pushs happen
    pushRepeatB = 0;
  }
  if (!buttonB && pushTimeB + longPushB < millis()) {
    bPushed = true;
    pushRepeatB++;
    if (pushRepeatB < 5) {
      longPushB += 200; // repetition time of repeated pushs
    } else {
      longPushB += 100; // repetition time of repeated pushs
    }
  }


  if (selectedItem == NONE) {
    // ----------------------------------- no item is selected!
    //.................................... cycle through items if nothing is selected
    if (aPushed) { // choose another item
      item--; bup();
      if (item==4)item=0; // skip v,a,w minmax adjustments
    }
    if (bPushed) { // choose another item
      item++; bup();
      if (item==1)item=5; // skip v,a,w minmax adjustments
    }
    if (item == -1)item = ITEMS - 1;
    if (item == ITEMS)item = 0;

    //.................................... select items
    if (cPushed) {
      selectedItem = item;
      bip();
    }
  } else {
    // ----------------------------------- an item is selected!
    //  .................................. time +-
    if (item == TIME) {
      newSecond = 0;
      if (aPushed) { // -
        bup();
        int repetitions = 1;
        if (pushRepeatA >= 5) repetitions = 10; // decrement 10 minutes if button is pushed long
        for (int i = 0; i < repetitions; i++) {
          newMinute--;
          if (newMinute < 0) {
            newMinute = 59;
            newHour--;
          }
          if (newHour < 0) {
            newHour = 23;
          }
        }
      }
      if (bPushed) { // +
        bup();
        int repetitions = 1;
        if (pushRepeatB >= 5) repetitions = 10; // increment 10 minutes if button is pushed long
        for (int i = 0; i < repetitions; i++) {
          newMinute++;
          if (newMinute > 59) {
            newMinute = 0;
            newHour++;
          }
          if (newHour > 23) {
            newHour = 0;
          }
        }
      }
      makeTimeString();
    }

    //  .................................. date +-
    if (item == DATE) {
      if (aPushed) { // -
        bup();
        int repetitions = 1;
        if (pushRepeatA >= 5) repetitions = 10; // increment 10 days
        for (int i = 0; i < repetitions; i++) {
          newDay--;
          if (newDay < 1) {
            newMonth--;
            newDay = 31;
            if (newMonth == 4 || newMonth == 6 || newMonth == 9 || newMonth == 11)newDay = 30;
            if (newMonth == 2) {
              newDay = 28;
              if (newYear % 4 == 0)newDay = 29;
            }
            if (newMonth < 1) {
              newMonth = 12;
              newYear--;
              if (newYear < 20)newYear = 20;
            }
          }
        }
      }
      if (bPushed) { // +
        bup();
        int repetitions = 1;
        if (pushRepeatB >= 5) repetitions = 10; // increment 10 days
        for (int i = 0; i < repetitions; i++) {
          newDay++;
          if (newDay == 31 && (newMonth == 4 || newMonth == 6 || newMonth == 9 || newMonth == 11)) {
            newDay = 1;
            newMonth++;
          }
          if (newDay == 32 && (newMonth == 1 || newMonth == 3 || newMonth == 5 || newMonth == 7 || newMonth == 8 || newMonth == 10 || newMonth == 12)) {
            newDay = 1;
            newMonth++;
            if (newMonth == 13) {
              newMonth = 1;
              newYear++;
            }
          }
          if (newMonth == 2) {
            if (newYear % 4 == 0 && newDay == 30) {
              newDay = 1;
              newMonth++;
            }
            if (newYear % 4 != 0 && newDay == 29) {
              newDay = 1;
              newMonth++;
            }
          }
        }
      }
      makeDateString();
    }

  /*  //  .................................. minV +-
    if (item == MIN_V) {
      if (aPushed) { // -
        minVolt--; bup();
        updateMinV = true;
      }
      if (bPushed) { // +
        minVolt++; bup();
        updateMinV = true;
      }
      minVolt = constrain(minVolt, 0, maxVolt - 1);
    }
    //  .................................. maxV +-
    if (item == MAX_V) {
      if (aPushed) { // -
        maxVolt--; bup();
        updateMaxV = true;
      }
      if (bPushed) { // +
        maxVolt++; bup();
        updateMaxV = true;
      }
      maxVolt = constrain(maxVolt, minVolt + 1, 70);
    }
    //  .................................. maxA +-
    if (item == MAX_A) {
      if (aPushed) { // -
        maxAmpere--; bup();
        updateMaxA = true;
      }
      if (bPushed) { // +
        maxAmpere++; bup();
        updateMaxA = true;
      }
      maxAmpere = constrain(maxAmpere, 1, 70);
    }
    //  .................................. maxW +-
    if (item == MAX_W) {
      if (aPushed) { // -
        bup();
        int repetitions = 1;
        if (pushRepeatA >= 5) repetitions = 10; // decrement 10 minutes if button is pushed long
        for (int i = 0; i < repetitions; i++) {
          maxWatt--;
          maxWatt = constrain(maxWatt, 1, 999);
        }
        updateMaxW = true;
      }
      if (bPushed) { // +
        bup();
        int repetitions = 1;
        if (pushRepeatB >= 5) repetitions = 10; // decrement 10 minutes if button is pushed long
        for (int i = 0; i < repetitions; i++) {
          maxWatt++;
          maxWatt = constrain(maxWatt, 1, 999);
        }
        updateMaxW = true;
      }
    }*/
    //  .................................. deselect item
    if (cPushed) {
      if (selectedItem == TIME || selectedItem == DATE) setNewTime();
      //if (selectedItem == MAX_V || selectedItem == MIN_V || selectedItem == MAX_W || selectedItem == MAX_A || selectedItem == LOG) writeEeprom();
      selectedItem = NONE;
      bip();
    }
  }
}
