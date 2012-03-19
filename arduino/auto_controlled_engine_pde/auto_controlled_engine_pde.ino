/*
 *  Macro Calculations
 */
const int infoLedPin = 13;
const int speedControlPin = 0; // A0
const int motorControlPin = 3; // D3
const int buttonPin = 8; // D8

const int __analogWrite_STEPS_NUM__ = 255;  // 0..255
const int __analogRead_STEPS_NUM__  = 1023; // 0..1023
const int __analogWrite_MAX_VAL__   = __analogWrite_STEPS_NUM__ + 1;
const int __analogRead_MAX_VAL__    = __analogRead_STEPS_NUM__  + 1;
const double __analogWriteToRead_RATIO__ = (double) __analogWrite_MAX_VAL__ / __analogRead_MAX_VAL__;
const int MIN_SPEED = 100;
const int MAX_SPEED = 254;
const double NORM_SLOPE = (double) (MAX_SPEED - MIN_SPEED) / __analogWrite_STEPS_NUM__;

/*
 *  Makes the argument for the Pulse-Width Modulation
 */
int getSpeed() { // via analogRead
  return analogRead(speedControlPin) * NORM_SLOPE * __analogWriteToRead_RATIO__ + MIN_SPEED;
}

double makeSpeed(int percentage) { // via the persentage from available range (maximum - minimum)
  return (double) 0.01 * percentage * (MAX_SPEED - MIN_SPEED) + MIN_SPEED;
}

// These variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int speedie = MIN_SPEED;     // variable for reading the current speed
int powerPercentage = 0;

double makePowerPercentage() {
    return (double) 100 * (speedie - MIN_SPEED) / (MAX_SPEED - MIN_SPEED);
}

void update() { analogWrite(motorControlPin, speedie); }

void linearSpeedChange(int end_speed, int speed_step_time) {
  if (end_speed >= powerPercentage) {
    for (int i = powerPercentage; i < end_speed; i++)
    {
      speedie = makeSpeed(i);
      update();
      delay(speed_step_time);
    }
  }
  else {
    for (int i = powerPercentage; i > end_speed; i--)
    {
      speedie = makeSpeed(i);
      update();
      delay(speed_step_time);
    }
  }
  powerPercentage = makePowerPercentage();
  Serial.println(speedie);
}

void setup() {
  Serial.begin(9600);
  pinMode(infoLedPin, OUTPUT);
  pinMode(motorControlPin, OUTPUT);
  pinMode(speedControlPin, INPUT);
  pinMode(buttonPin, INPUT);

  MotorControllerInit();
}

void MotorControllerInit() {
  
  // Относительный нуль
  update();
  Serial.println(speedie);
  
  // Ожидание инициализации конроллера мотора + три писка
  delay(8000);
  
  // Плавное увеличение оборотов от относительного нуля до максимума
  // (линейно от 100 до 254 за 2000 мс)
  linearSpeedChange(100, 50);
  
  // Плавное уменьшение оборотов до уровня "50%" (штатный режим)
  // (линейно от 254 до 177 за 1000 мс)
  linearSpeedChange(50, 50);
}

void MotorControllerStop() {
  linearSpeedChange(0, 100);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    MotorControllerStop();
  }
  
  delay(50);
}

