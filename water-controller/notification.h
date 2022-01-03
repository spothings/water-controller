// variable for notification
bool voltageStatusOld = true;
bool voltageStatusNew = true;
bool ultrasonikStatusOld = true;
bool ultrasonikStatusNew = true;
bool waterflowStatusOld = true;
bool waterflowStatusNew = true;

void serial_monitor(){
    Serial.print(selenoid); Serial.print(" : ");
    Serial.print(distance); Serial.print(" : ");
    Serial.print(waiting_on); Serial.print(" : ");
    Serial.print(waiting_off); Serial.print(" : ");
    Serial.print(voltage); Serial.print(" : ");
    Serial.print(waterflow); Serial.print(" : ");
    Serial.print(maxWater); Serial.print(" : ");
    Serial.print(minVoltage); Serial.print("\n");
}

void baterai_status(){
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
}

void ultrasonic_status(){
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
}

void waterflow_status(){
    // Notify for waterflow
    if(waterflowStatusNew != waterflowStatusOld){
      if(selenoid_status && waterflow == 0.00){
        bot.sendMessage(CHAT_ID, "Selenoid or waterflow have error", "");
      } else if(waterflow != 0.00 && !selenoid_status){
        bot.sendMessage(CHAT_ID, "Selenoid or waterflow have error", "");
      }
      waterflowStatusOld = waterflowStatusNew;
    } else {
      if(selenoid_status && waterflow != 0.00 || !selenoid_status && waterflow == 0.00){
        waterflowStatusNew = true;
      } else {
        waterflowStatusNew = false;
      }
    }
}
