#include "secret.h"
#include "wifi.h"
#include "ultrasonic.h"

TaskHandle_t Task1;
TaskHandle_t Task2;

int waiting_on = 0;
int waiting_off = 0;
bool status = false;

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/reset") {
      String welcome = "Hallo, " + from_name + ".\n";
      welcome += "Silahkan atur jarak ketinggian air\n\n";
      bot.sendMessage(CHAT_ID, welcome, "");
      loop_ultrasonik();
      box_high = distanceCm;
      String set_distance = "Ketinggian air sudah diatur : " + String(box_high) + ".\n";
      bot.sendMessage(CHAT_ID, set_distance, "");
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  setup_ultrasonic();

  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */ 
  delay(500);
  
  xTaskCreatePinnedToCore(
    Task2code,   /* Task function. */
    "Task2",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task2,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  delay(500);
}

void Task1code( void * pvParameters ){
  for(;;){
    loop_ultrasonik();
    Serial.print("Distance : ");
    Serial.print(distanceCm);
    Serial.print(" : ");
    Serial.print(waiting_on);
    Serial.print(" : ");
    Serial.print(waiting_off);
    if(status){
      Serial.println(" : Selenoid On");
    }else{
      Serial.println(" : Selenoid Off");
    }
    
    if(distanceCm > box_high){
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

void Task2code( void * pvParameters ){
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
