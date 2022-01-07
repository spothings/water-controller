#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
bool teleMonitor = false;
unsigned long lastTimeBotRan;

void setup_telegram(){
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  // send message to telegram on startup
  bot.sendMessage(CHAT_ID, "Bot started up\ntype /on to turn on device", "");
}

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

    if (text == "/reset_max") {
      String textMessage = "Silahkan atur jarak ketinggian air\n\n";
      bot.sendMessage(CHAT_ID, textMessage, "");

      maxWater = write_maxWater(distance);
      Serial.println(distance);
      Serial.println(maxWater);
      String set_distance = "Ketinggian maksimal air sudah diatur : " + String(distance) + " CM.\n";
      bot.sendMessage(CHAT_ID, set_distance, "");
    }

    if (text == "/reset_min") {
      String textMessage = "Silahkan atur jarak minimal air\n\n";
      bot.sendMessage(CHAT_ID, textMessage, "");
  
      minWater = write_minWater(distance);
      Serial.println(distance);
      Serial.println(minWater);
      
      String set_distance = "Ketinggian minimal air sudah diatur : " + String(distance) + " CM.\n";
      bot.sendMessage(CHAT_ID, set_distance, "");
    }
    
    if (text == "/info") {
      String information = "Ketinggian air maksimal : " + String(maxWater) + ".\n";
      information += "Ketinggian air saat ini : " + String(distance) + ".\n";
      information += "Baterai voltase : " + String(voltage) + "V.\n";
      String selenoidStatus;
      if(selenoid_status){
        selenoidStatus = "On";
      }else{
        selenoidStatus = "Off";
      }
      information += "Selenoid status : " + selenoidStatus + ".\n";
      bot.sendMessage(CHAT_ID, information, "");
    }

    if (text == "/monitor") {
      String printMonitor;
      if(teleMonitor){
        printMonitor = "Monitoring off";
        teleMonitor = false;
      } else {
        printMonitor = "Selenoid,\tDistance,\tWaitingOn,\tWaitingOff,\tWaterflow,\tmaxWater,\tminWater\n";
        teleMonitor = true;
      }
      bot.sendMessage(CHAT_ID, printMonitor, "");
    }

    if (text == "/off") {
      selenoid_status = false;
      selenoid_on_tele = false;
      bot.sendMessage(CHAT_ID, "Device Off", "");
    }

    if (text == "/on") {
      selenoid_status = true;
      selenoid_on_tele = true;
      bot.sendMessage(CHAT_ID, "Device On", "");
    }

    if (text == "/force_on") {
      selenoid_status = true;
      selenoid_on_tele = true;
      digitalWrite(selenoidPin, HIGH);
      bot.sendMessage(CHAT_ID, "Device force On", "");
    }
  }
}

void loop_telegram(){
    if (millis() > lastTimeBotRan + botRequestDelay)  {
      int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  
      while(numNewMessages) {
        Serial.println("got response");
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
      lastTimeBotRan = millis();
    }

    if(teleMonitor){
      String printMonitor = String(selenoid) + " \t: " + String(distance) + "CM \t: " + String(waiting_on) + " \t: " + String(waiting_off) + " \t: " + 
      String(voltage) + "V \t: " + String(waterflow) + "L/m \t: " + String(maxWater) + "CM \t: " + String(minWater) +"CM\n";

      bot.sendMessage(CHAT_ID, printMonitor, "");
    }
}
