// Measures flow sensor pulses
volatile int flow_frequency;

// Calculated litres/hour
float vol = 0.0,l_minute;

// Sensor Input
unsigned char flowsensor = 15; // 15 - 2
unsigned long currentTime;
unsigned long cloopTime;

// Interrupt function
void flow (){
   flow_frequency++;
}

void setup_waterflow(){
   pinMode(flowsensor, INPUT);

   // Optional Internal Pull-Up
   digitalWrite(flowsensor, HIGH);

   // Setup Interrupt
   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING);
   currentTime = millis();
   cloopTime = currentTime;
}

double loop_waterflow (){
  double correntflow;
  currentTime = millis();
  
  // Every second, calculate and print litres/hour
  if(currentTime >= (cloopTime + 1000)){
    
    // Updates cloopTime
    cloopTime = currentTime;
    if(flow_frequency != 0){
      
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      l_minute = l_minute/60;
      vol = vol +l_minute;

      // Reset Counter
      flow_frequency = 0;

      // return value
      correntflow = 60 * l_minute, DEC;
      return correntflow;
    } else {
      return 0;
    }
  }
}
