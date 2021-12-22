#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

void setup_telegram(){
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  // send message to telegram on startup
  bot.sendMessage(CHAT_ID, "Bot started up", "");
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

    if (text == "/reset") {
      String welcome = "Hallo, " + from_name + ".\n";
      welcome += "Silahkan atur jarak ketinggian air\n\n";
      bot.sendMessage(CHAT_ID, welcome, "");
      box_high = distance;
      String set_distance = "Ketinggian air sudah diatur : " + String(box_high) + ".\n";
      bot.sendMessage(CHAT_ID, set_distance, "");
    }
  }
}
