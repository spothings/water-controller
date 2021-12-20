#include "secret.h"
#include "wifi.h"
#include "ultrasonic.h"

int waiting_on = 0;
int waiting_off = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  setup_ultrasonic();
}

void loop() {
  loop_ultrasonik();
  Serial.print("Distance : ");
  Serial.print(distanceCm);
  Serial.print(" : ");
  Serial.print(waiting_on);
  Serial.print(" : ");
  Serial.println(waiting_off);
  
  if(distanceCm > 10){
    if(waiting_on >= 5){
      Serial.println("Selenoid On");
    }
    waiting_on += 1;
    waiting_off = 0;
  }else{
    if(waiting_off >= 5){
      Serial.println("Selenoid Off");
    }
    waiting_on = 0;
    waiting_off += 1;
  }
  delay(1000);
}
