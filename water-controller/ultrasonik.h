#define SOUND_SPEED 0.034

const int trigPin = 5;
const int echoPin = 18;

void setup_ultrasonic(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

float loop_ultrasonik(){
  long duration;
  float distance;
  
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distance = duration * SOUND_SPEED/2;

  return distance;
}
