#include <EEPROM.h>
#define EEPROM_SIZE 512

int box_high;

void setup_storage(){
  EEPROM.begin(EEPROM_SIZE);
  
  box_high = EEPROM.read(0);
}

void loop_storage(){
  box_high = EEPROM.read(0);
}

int write_storage(int distance){
  EEPROM.write(0, distance);
  EEPROM.commit();
  return distance;
}
