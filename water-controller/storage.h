#include <EEPROM.h>
#define EEPROM_SIZE 2

float maxWater;
float minWater;

void setup_storage(){
  EEPROM.begin(EEPROM_SIZE);
  
  maxWater = EEPROM.read(0);
  minWater = EEPROM.read(1);
}

void loop_storage(){
  maxWater = EEPROM.read(0);
  minWater = EEPROM.read(1);
}

float write_maxWater(float distance){
  EEPROM.write(0, distance);
  EEPROM.commit();
  return distance;
}

float write_minWater(float distance){
  EEPROM.write(1, distance);
  EEPROM.commit();
  return distance;
}
