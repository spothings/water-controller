float distance = 0.0;

// declarate to multi core
TaskHandle_t Message;
TaskHandle_t Scan;

// variable relay/selenoid
int waiting_on = 0;
int waiting_off = 0;
const int selenoidPin = 26;
bool selenoid_status = false;

// variable voltage sensor
const int voltagePin = 34;
bool voltageStatusOld = true;
bool voltageStatusNew = true;

#include "secret.h"
#include "wifi.h"
#include "ultrasonik.h"
#include "waterflow.h"
#include "telegram.h"

void setup() {
  Serial.begin(115200);
  
  setup_wifi();
  setup_ultrasonic();
  setup_waterflow();
  setup_telegram();

  // set pin selenoid as output
  pinMode(selenoidPin, OUTPUT);

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
    Serial.print("Distance : ");
    Serial.print(distance);
    Serial.print(" : ");
    Serial.print(waiting_on);
    Serial.print(" : ");
    Serial.print(waiting_off);
    Serial.print(" : ");
    Serial.print(box_high);
    Serial.print(" : ");
    Serial.print(analogRead(voltagePin));
    
    if(selenoid_status){
      digitalWrite(selenoidPin, HIGH);
      Serial.print(" : Selenoid On : ");
    }else{
      digitalWrite(selenoidPin, LOW);
      Serial.print(" : Selenoid Off : ");
    }
    
    if(distance > box_high){
      if(waiting_on >= 5){
        selenoid_status = true;
      }
      waiting_on += 1;
      waiting_off = 0;
    }else{
      if(waiting_off >= 5){
        selenoid_status = false;
      }
      waiting_on = 0;
      waiting_off += 1;
    }

    loop_waterflow();
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

    if(voltageStatusNew != voltageStatusOld){
      if(!voltageStatusNew){
        bot.sendMessage(CHAT_ID, "Baterai is Low", "");
      }
      voltageStatusOld = voltageStatusNew;
    }else{
      if(analogRead(voltagePin) > minVoltage){
        voltageStatusNew = true;
      }else{
        voltageStatusNew = false;
      }
    }
  }
}

void loop() {
  
}
