// variable voltage sensor
const int voltagePin = 35; // 35 - 34
int voltageValue;

const int minVoltage = 8;

int loop_voltage(){
  voltageValue = analogRead(voltagePin);
  voltageValue = voltageValue/215;
  return voltageValue;
}
