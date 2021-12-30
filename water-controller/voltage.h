// variable voltage sensor
const int voltagePin = 34;
int voltageValue;

const int minVoltage = 2300;

int loop_voltage(){
  voltageValue = analogRead(voltagePin);
  return voltageValue;
}
