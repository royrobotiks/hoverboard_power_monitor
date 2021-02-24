

void checkTime() {
  readRTC();
  makeDateString();
  makeTimeString();
}

void readRTC() {
  DateTime now = rtc.now();
  if (selectedItem != DATE) { // only update if date is not selected
    newYear = now.year();
    newMonth = now.month();
    newDay = now.day();
  }

  if (selectedItem != TIME) { // only update if time is not selected
    newHour = now.hour();
    newMinute = now.minute();
    newSecond = now.second();
  }
}

void makeDateString() {
  sprintf(dateString, "%02d/%02d/%02d", (newYear - 2000), newMonth, newDay);
}

void makeTimeString() {
  sprintf(timeString, "%02d:%02d:%02d", newHour, newMinute, newSecond);
}

void setNewTime() {
  rtc.adjust(DateTime(newYear, newMonth, newDay, newHour, newMinute, 0));
}
