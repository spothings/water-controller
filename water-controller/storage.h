#include <EEPROM.h>
#define EEPROM_SIZE 8

float maxWater;
float minWater;

void setup_storage(){
  EEPROM.begin(EEPROM_SIZE);
  
  maxWater = EEPROM.readFloat(0);
  minWater = EEPROM.readFloat(4);
}

void loop_storage(){
  maxWater = EEPROM.readFloat(0);
  minWater = EEPROM.readFloat(4);
}

float write_maxWater(float distance){
  EEPROM.writeFloat(0, distance);
  EEPROM.commit();
  maxWater = EEPROM.readFloat(0);
  return maxWater;
}

float write_minWater(float distance){
  EEPROM.writeFloat(4, distance);
  EEPROM.commit();
  minWater = EEPROM.readFloat(4);
  return minWater;
}
