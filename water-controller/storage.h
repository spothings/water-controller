#include <EEPROM.h>
#define EEPROM_SIZE 2

int maxWater;
int minWater;

void setup_storage(){
  EEPROM.begin(EEPROM_SIZE);
  
  maxWater = EEPROM.read(0);
  minWater = EEPROM.read(1);
}

void loop_storage(){
  maxWater = EEPROM.read(0);
  minWater = EEPROM.read(1);
}

int write_maxWater(int distance){
  EEPROM.write(0, distance);
  EEPROM.commit();
  return distance;
}

int write_minWater(int distance){
  EEPROM.write(1, distance);
  EEPROM.commit();
  return distance;
}
