
boolean init_SD() {
  // try to init SD card
  //Serial.print("SD card init...");
  boolean returnValue = false;
  if (SD.begin(SD_CS)) {
    //Serial.println("Success!");
    returnValue = true;
  } else {
    //Serial.println("Failed!");
  }
  return returnValue;
}


// log data on sd card. format for each line is millis, date, time, v, a, w

void sdLog() {
  boolean success = false;
  if (pSelectedItem != LOG) { // if log is starting, try to open sd card
    sdFail = !init_SD();
    if (!sdFail) { // init new file
      fileNumber ++;
      writeEeprom();
      sprintf (fileName, "PWR_%04d.LOG", fileNumber);
      logStart = millis();
    }
  }

  if (!sdFail) { // open file and write to it if it is available
    File logFile = SD.open(fileName, FILE_WRITE);
    if (logFile) {
      success = true;
      char logData[55];
      if (pSelectedItem != LOG) { // if logfile is new write description to columns
        logFile.println("millisecs |yr|mo|da|hr|mi|se|volt   |ampere |watt   ");
      }
      sprintf(logData,  "%010ld|%02d|%02d|%02d|%02d|%02d|%02d|%03d.%03d|%03d.%03d|%03d.%03d",
              (millis() - logStart), newYear - 2000, newMonth, newDay, newHour, newMinute, newSecond,
              int(volt),  int(volt * 1000) - (int(volt) * 1000),
              int(ampere),  int(ampere * 1000) - (int(ampere) * 1000),
              int(watt),  int(watt * 1000) - (int(watt) * 1000)
             );

      logFile.println(logData);
      logFile.close();
    }
  }
}
