// variable for notification
bool voltageStatusOld = true;
bool voltageStatusNew = true;
bool ultrasonikStatusOld = true;
bool ultrasonikStatusNew = true;
bool waterflowStatusOld = true;
bool waterflowStatusNew = true;
bool baterai_indikator = true;

void serial_monitor(){
    Serial.print(selenoid); Serial.print(" : ");
    Serial.print(distance); Serial.print("CM : ");
    Serial.print(waiting_on); Serial.print(" : ");
    Serial.print(waiting_off); Serial.print(" : ");
    Serial.print(voltage); Serial.print("V : ");
    Serial.print(waterflow); Serial.print("L/m : ");
    Serial.print(maxWater); Serial.print("CM : ");
    Serial.print(minWater); Serial.print("CM\n");
}

void baterai_status(){
  // Notify for baterai status  
    if(voltageStatusNew != voltageStatusOld){
      if(baterai_indikator){
        if(!selenoid_status && voltage == 0){
          bot.sendMessage(CHAT_ID, "Baterai is Missing", "");
          baterai_indikator = false;
        } else if(!selenoid_status && voltage < minVoltage){
          bot.sendMessage(CHAT_ID, "Baterai is Low", "");
          baterai_indikator = false;
        }
      }else{
        if(!selenoid_status){
          bot.sendMessage(CHAT_ID, "Baterai is Good", "");
          baterai_indikator = true;
        }
      }
      voltageStatusOld = voltageStatusNew;
    }else{
      if(!selenoid_status && voltage >= minVoltage){
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
      if(selenoid_status){
        int time_to_full = time_water();
        bot.sendMessage(CHAT_ID, "Water On", "");
        bot.sendMessage(CHAT_ID, "Full estimate " + String(time_to_full) + " Second", "");
      }
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
