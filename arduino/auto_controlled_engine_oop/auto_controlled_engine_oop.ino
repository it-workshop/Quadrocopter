/*
 *  Macro Calculations and global constants
 */
const int infoLedPin = 13;
const int speedControlPin = 0; // A0
const int motorControlPin = 3; // D3
const int A_buttonPin = 7; // D8
const int B_buttonPin = 8; // D8

class MotorController {
private:
	static const int INIT_TIMEOUT = 8000; // ms
	static const int DEF_SPEED_STEP = 50;
	
	enum EXTREMAL_SPEED {
		MIN_SPEED = 100,
		MAX_SPEED = 254
	};
	
	int speed_;
	const char* name_;
	const int motor_control_pin_;
	
	inline const int power();
	
	inline const double makeSpeed(int percentage);
	
	void update();
	
public:
	MotorController(const char* name, const int motor_control_pin);
	~MotorController();
	
	void linearSpeedChange(int end_speed, int speed_step_time);
	void linearSpeedInc(int inc_percent, int speed_step_time);
};


	
inline const int MotorController::power()
{
	return (double) 100 * (speed_ - MIN_SPEED) / (MAX_SPEED - MIN_SPEED);
}
	
inline const double MotorController::makeSpeed(int percentage)
{
	if (percentage >= 100) return MAX_SPEED;
        if (percentage <=   0) return MIN_SPEED;
	return (double) 0.01 * percentage * (MAX_SPEED - MIN_SPEED) + MIN_SPEED;
}
	
void MotorController::update()
{
	analogWrite(motor_control_pin_, speed_);
}
	
void MotorController::linearSpeedChange(int end_speed, int speed_step_time)
{
	if (end_speed > power())
	for (int i = power(); i < end_speed; i++)
	{
		speed_ = makeSpeed(i);
		update();
                if (speed_ == MAX_SPEED) return;
		delay(speed_step_time);
	}
	
	else
	for (int i = power(); i > end_speed; i--)
	{
		speed_ = makeSpeed(i);
		update();
                if (speed_ == MIN_SPEED) return;
		delay(speed_step_time);
	}
}

MotorController::MotorController(const char* name, const int motor_control_pin)
: speed_ (MIN_SPEED),
name_ (name),
motor_control_pin_ (motor_control_pin)
{
	update();
	
	// Ожидание инициализации конроллера мотора + три писка
	delay(INIT_TIMEOUT);
	
	// Плавное увеличение оборотов от относительного нуля до 50%
	linearSpeedChange(50, DEF_SPEED_STEP);
}

// Увеличить обороты двигателя на inc_percent (за время, кратное speed_step_time)
void MotorController::linearSpeedInc(int inc_percent, int speed_step_time)
{
    linearSpeedChange(power() + inc_percent, speed_step_time);
}

MotorController* motor;

void setup()
{
  Serial.begin(9600);
  pinMode(infoLedPin, OUTPUT);
  pinMode(motorControlPin, OUTPUT);
  pinMode(speedControlPin, INPUT);
  pinMode(A_buttonPin, INPUT);
  pinMode(B_buttonPin, INPUT);
  
  // init the global object
  motor = new MotorController("My Super Motor", motorControlPin);
}

void loop()
{
  if (digitalRead(A_buttonPin) == HIGH) {
    digitalWrite(infoLedPin, HIGH);
    motor->linearSpeedInc(-20, 200);
    digitalWrite(infoLedPin, LOW);
  }
  
  if (digitalRead(B_buttonPin) == HIGH) {
    digitalWrite(infoLedPin, HIGH);
    motor->linearSpeedInc(20, 200);
    digitalWrite(infoLedPin, LOW);
  }
  
  delay(50);
}

