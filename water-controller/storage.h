#include <EEPROM.h>
#define EEPROM_SIZE 512

int maxWater;

void setup_storage(){
  EEPROM.begin(EEPROM_SIZE);
  
  maxWater = EEPROM.read(0);
}

void loop_storage(){
  maxWater = EEPROM.read(0);
}

int write_storage(int distance){
  EEPROM.write(0, distance);
  EEPROM.commit();
  return distance;
}
