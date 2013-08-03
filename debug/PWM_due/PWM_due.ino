const int PWMPin = 9;
const int PWMValue = 100;

void setup() {
  // put your setup code here, to run once:
  pinMode(PWMPin, OUTPUT);
  analogWrite(PWMPin, PWMValue);
}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
