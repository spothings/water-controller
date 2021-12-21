#include "secret.h"
#include "wifi.h"
#include "ultrasonik.h"
#include "telegram.h"

TaskHandle_t Scan;
TaskHandle_t Message;

int waiting_on = 0;
int waiting_off = 0;
bool status = false;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  setup_ultrasonic();
  setup_telegram();

  xTaskCreatePinnedToCore(ScanCode, "Scan", 10000, NULL, 1, &Scan, 0);
  delay(500);
  
  xTaskCreatePinnedToCore(MessageCode, "Message", 10000, NULL, 1, &Message, 1);
  delay(500);
}

void ScanCode( void * pvParameters ){
  for(;;){
    loop_ultrasonik();
    Serial.print("Distance : ");
    Serial.print(distance);
    Serial.print(" : ");
    Serial.print(waiting_on);
    Serial.print(" : ");
    Serial.print(waiting_off);
    Serial.print(" : ");
    Serial.print(box_high);
    
    if(status){
      Serial.println(" : Selenoid On");
    }else{
      Serial.println(" : Selenoid Off");
    }
    
    if(distance > box_high){
      if(waiting_on >= 5){
        status = true;
      }
      waiting_on += 1;
      waiting_off = 0;
    }else{
      if(waiting_off >= 5){
        status = false;
      }
      waiting_on = 0;
      waiting_off += 1;
    }
    delay(1000);
  } 
}

void MessageCode( void * pvParameters ){
  for(;;){
    if (millis() > lastTimeBotRan + botRequestDelay)  {
      int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  
      while(numNewMessages) {
        Serial.println("got response");
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
      lastTimeBotRan = millis();
    }
  }
}

void loop() {
  
}
