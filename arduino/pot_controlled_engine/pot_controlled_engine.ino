const int infoLedPin = 13;
const int speedControlPin = 0; // A0
const int motorControlPin = 3; // D3

const int __analogWrite_STEPS_NUM__ = 255;  // 0..255
const int __analogRead_STEPS_NUM__  = 1023; // 0..1023
const int __analogWrite_MAX_VAL__   = __analogWrite_STEPS_NUM__ + 1;
const int __analogRead_MAX_VAL__    = __analogRead_STEPS_NUM__  + 1;
const double __analogWriteToRead_RATIO__ = (double) __analogWrite_MAX_VAL__ / __analogRead_MAX_VAL__;
const int MIN_SPEED = 100;
const int MAX_SPEED = 254;
const double NORM_SLOPE = (double) (MAX_SPEED - MIN_SPEED) / __analogWrite_STEPS_NUM__;

int getSpeed() {
  return analogRead(speedControlPin) * NORM_SLOPE * __analogWriteToRead_RATIO__ + MIN_SPEED;
}
 
int speedie = 0;

void setup() {
  Serial.begin(9600);
  pinMode(infoLedPin, OUTPUT);
  pinMode(motorControlPin, OUTPUT);
  pinMode(speedControlPin, INPUT);
}

void loop() {
  speedie = getSpeed();
  analogWrite(motorControlPin, speedie);
  Serial.println(speedie);
  //delay(10);
}

