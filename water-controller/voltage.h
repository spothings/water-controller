// variable voltage sensor
const int voltagePin = 34;
int voltageValue;

int loop_voltage(){
  voltageValue = analogRead(voltagePin);
  return voltageValue;
}
