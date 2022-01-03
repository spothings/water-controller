int voltage;
float distance;
double waterflow;
String selenoid;

// declarate to multi core
TaskHandle_t Message;
TaskHandle_t Scan;

#include "secret.h"
#include "wifi.h"
#include "storage.h"
#include "voltage.h"
#include "ultrasonik.h"
#include "waterflow.h"
#include "selenoid.h"
#include "telegram.h"
#include "notification.h"

void setup() {
  Serial.begin(115200);
  
  setup_wifi();
  setup_storage();
  setup_ultrasonic();
  setup_waterflow();
  setup_selenoid();
  setup_telegram();

  // configure core 1 (handle messages)
  xTaskCreatePinnedToCore(MessageCode, "Message", 10000, NULL, 1, &Message, 0);
  delay(500);

  // configure core 2 (scan water)
  xTaskCreatePinnedToCore(ScanCode, "Scan", 10000, NULL, 1, &Scan, 1);
  delay(500);
}

void ScanCode( void * pvParameters ){
  for(;;){
    distance = loop_ultrasonik();
    waterflow = loop_waterflow();
    voltage = loop_voltage();
    selenoid = loop_selenoid();

    seril_monitor();
    
    delay(1000);
  } 
}

void MessageCode( void * pvParameters ){
  for(;;){
    loop_telegram();
    loop_storage();

    baterai_status();
    ultrasonic_status();
    waterflow_status();
  }
}

void loop() {
  
}
