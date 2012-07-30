/*
 *  Macro Calculations and global constants
 */

const int EN = 2;

const int infoLedPin = 13;
const int A_buttonPin = 7; // D7
const int B_buttonPin = 8; // D8

struct RVector3D {
	double x, y, z;
	double module_sq();
        void norm();
        void set_module();
        void x_angle_inc(double w);
        void y_angle_inc(double w);
        void x_angle_dec(double w);
        void y_angle_dec(double w);
};

double RVector3D::module_sq()
{
	return x * x + y * y + z * z;
}

void RVector3D::norm()
{
	double k = 1.0 / sqrt(module_sq());
        x *= k;
        y *= k;
        z *= k;
}

void RVector3D::x_angle_inc(double w)
{
         y = y * cos(w) - z * sin(w);
         z = y * sin(w) + z * cos(w);
}

void RVector3D::x_angle_dec(double w)
{
         x_angle_inc(-w);
}

void RVector3D::y_angle_inc(double w)
{
         x =  x * cos(w) + z * sin(w);
         z = -x * sin(w) + z * cos(w);
}

void RVector3D::y_angle_dec(double w)
{
         y_angle_inc(-w);
}

struct Motor {
	enum EXTREMAL_SPEED {
		MIN_SPEED = 100,
		MAX_SPEED = 254
	};
	int control_pin;
	int speedie;
	inline const int power();
	void makeSpeed(int percentage);
};

inline const int Motor::power()
{
	return (double) 100 * (speedie - MIN_SPEED) / (MAX_SPEED - MIN_SPEED);
}

void Motor::makeSpeed(int percentage)
{
	if (percentage >= 100) speedie = MAX_SPEED;
	if (percentage <=   0) speedie = MIN_SPEED;
	speedie = (double) 0.01 * percentage * (MAX_SPEED - MIN_SPEED) + MIN_SPEED;
        analogWrite(control_pin, speedie);
        Serial.println(speedie);
}

class MotorController {
private:
	static const int INIT_TIMEOUT = 8000; // ms
	static const int DEF_SPEED_STEP = 400;

	enum SIGN {
		ZERO = 0,
		PLUS = 1,
		MINUS = -1
	};
	
	enum MOTORS {
		A, B, C, D, N_MOTORS
	};
	
	Motor motors_[N_MOTORS];	
	
	inline const SIGN x_sign(int i)
	{
		switch (i) {
  		case A: return MINUS;
  		case B: return PLUS;
  		case C: return PLUS;
  		case D: return MINUS;
  		default: return ZERO;
		}
	}
	
	inline const SIGN y_sign(int i)
	{
		switch (i) {
		case A: return MINUS;
		case B: return MINUS;
		case C: return PLUS;
		case D: return PLUS;
		default: return ZERO;
		}
	}

public:
	MotorController(const int motor_control_pins[N_MOTORS]);
	~MotorController();
	
	void speedChange(RVector3D throttle_vec);
	//void linearSpeedInc(int inc_percent, int speed_step_time);
};



void MotorController::speedChange(RVector3D throttle_vec)
{
	for (int i = 0; i < N_MOTORS; i ++)
	{	
                // This comes from the Cubic Vector Model
                double power = 100 * ( throttle_vec.module_sq() + x_sign(i) * throttle_vec.x + y_sign(i) * throttle_vec.y ) / throttle_vec.z;
                Serial.println("motor #");
                Serial.println(i);
                Serial.println("end power = ");
                Serial.println(power);
	        motors_[i].makeSpeed(power);
        }
}

MotorController::MotorController(const int motor_control_pins[N_MOTORS])
{
	for (int i = 0; i < N_MOTORS; i ++)
	{
		motors_[i].control_pin = motor_control_pins[i];
		motors_[i].makeSpeed(0);
	}
	
	// Ожидание инициализации конроллов моторов + примерно три писка
	delay(INIT_TIMEOUT);
	
	RVector3D start_throttle;
	start_throttle.x = 0;
	start_throttle.y = 0;
	start_throttle.z = 0.1;
	speedChange(start_throttle);
}

/*
// Увеличить обороты двигателя на inc_percent (за время, кратное speed_step_time)
void MotorController::linearSpeedInc(int inc_percent, int speed_step_time)
{
    linearSpeedChange(power() + inc_percent, speed_step_time);
}
*/

MotorController* MController;
RVector3D throttle;

void setup()
{
	Serial.begin(9600);
	pinMode(infoLedPin, OUTPUT);
	pinMode(A_buttonPin, INPUT);
	pinMode(B_buttonPin, INPUT);

        pinMode(3, OUTPUT);
        pinMode(10, OUTPUT);
        pinMode(11, OUTPUT);
        pinMode(9, OUTPUT);

	// init the global object
        int motor_control_pins[4] = {3, 10, 11, 9};
	MController = new MotorController(motor_control_pins);

        throttle.x = 0;
        throttle.y = 0;
        throttle.z = 1;
}

int command = 0;

void loop()
{
        // Enable Receiving Data
        digitalWrite(EN, LOW);

        // if we get a valid byte
        if (Serial.available() > 0) {

            command = Serial.read();
            switch (command) {
            case 'a':
              throttle.y_angle_inc(0.024543692); // PI / 128
              break;

            case 'd':
              throttle.y_angle_dec(0.024543692);
              break;

            case 'w':
              throttle.x_angle_dec(0.024543692);
              break; 

            case 's':
              throttle.x_angle_inc(0.024543692);
              break;
            
            case '1': case '2': case '3': case '4': 
            case '5': case '6': case '7': case '8': case '9': 
              double coef = (double) (command - 48) * 0.1;
              throttle.norm();
              throttle.x *= coef;
              throttle.y *= coef;
              throttle.z *= coef;
              break;
              
            case '0':
              throttle.x = 0;
              throttle.y = 0;
              throttle.z = 0;
              break;  
            }
            
            MController->speedChange(throttle);
        }
        
	delay(50);
}

