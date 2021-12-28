int voltage;
float distance;
double waterflow;
String selenoid;

// declarate to multi core
TaskHandle_t Message;
TaskHandle_t Scan;

// variable for notification
bool voltageStatusOld = true;
bool voltageStatusNew = true;
bool ultrasonikStatusOld = true;
bool ultrasonikStatusNew = true;

#include "secret.h"
#include "wifi.h"
#include "storage.h"
#include "voltage.h"
#include "ultrasonik.h"
#include "waterflow.h"
#include "selenoid.h"
#include "telegram.h"

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

    Serial.print(selenoid); Serial.print(" : ");
    Serial.print(distance); Serial.print(" : ");
    Serial.print(waiting_on); Serial.print(" : ");
    Serial.print(waiting_off); Serial.print(" : ");
    Serial.print(voltage); Serial.print(" : ");
    Serial.print(waterflow); Serial.print(" : ");
    Serial.print(box_high); Serial.print(" : ");
    Serial.print(minVoltage); Serial.print("\n");
    
    delay(1000);
  } 
}

void MessageCode( void * pvParameters ){
  for(;;){
    loop_telegram();
    loop_storage();

    // Notify for baterai status
    if(voltageStatusNew != voltageStatusOld){
      if(voltageStatusNew == 0.00){
        bot.sendMessage(CHAT_ID, "Baterai is Missing", "");
      } else if(!voltageStatusNew){
        bot.sendMessage(CHAT_ID, "Baterai is Low", "");
      } else {
        bot.sendMessage(CHAT_ID, "Baterai is Good", "");
      }
      voltageStatusOld = voltageStatusNew;
    }else{
      if(voltage > minVoltage){
        voltageStatusNew = true;
      }else{
        voltageStatusNew = false;
      }
    }
    
    // Notify for ultrasonic status
    if(ultrasonikStatusNew != ultrasonikStatusOld){
      if(ultrasonikStatusNew == 0.00){
        bot.sendMessage(CHAT_ID, "Ultrasonik is Missing", "");
      } else {
        bot.sendMessage(CHAT_ID, "Ultrasonik is Good", "");
      }
      ultrasonikStatusOld = ultrasonikStatusNew;
    }else{
      if(distance != 0.00){
        ultrasonikStatusNew = true;
      }else{
        ultrasonikStatusNew = false;
      }
    }

    // Notify for waterflow
//    if(selenoid_status && waterflow == 0){
//      bot.sendMessage(CHAT_ID, "Selenoid have error", "");
//    } else if(waterflow != 0 && !selenoid_status){
//      bot.sendMessage(CHAT_ID, "Waterflow have error", "");
//    }
  }
}

void loop() {
  
}
