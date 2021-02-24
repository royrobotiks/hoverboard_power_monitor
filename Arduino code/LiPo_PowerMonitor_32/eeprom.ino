
void writeEeprom() {
  // ----------------------------------------------------- write eeprom
  int addr = 0;
//  EEPROM.put(addr, minVolt); addr += sizeof(int);
//  EEPROM.put(addr, maxVolt); addr += sizeof(int);
//  EEPROM.put(addr, maxAmpere); addr += sizeof(int);
//  EEPROM.put(addr, maxWatt); addr += sizeof(int);
  EEPROM.put(addr, fileNumber);
}
