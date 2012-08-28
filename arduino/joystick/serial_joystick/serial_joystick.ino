const int SERIAL_SPEED = 9600;

const int APINS_N = 3;
const int APINS[APINS_N] = {A0, A1, A4};

const int DPINS_N = 1;
const int DPINS[DPINS_N] = {10};

const char C_REQUEST = 'r';

#define infoLedPin 13

//#define DEBUG

void setup()
{
    //power for key
    pinMode(8, OUTPUT);
    digitalWrite(8, HIGH);
    pinMode(9, OUTPUT);
    digitalWrite(9, HIGH);
    
    pinMode(6, OUTPUT);
    digitalWrite(6, LOW);
    pinMode(7, OUTPUT);
    digitalWrite(7, LOW);
    
    
    for(int i = 0; i < APINS_N; i++)
        pinMode(APINS[i], INPUT);
        
    for(int i = 0; i < DPINS_N; i++)
        pinMode(DPINS[i], INPUT);
        
    Serial.begin(SERIAL_SPEED);
}

void loop()
{
    static int i, t_int;
    char t_high, t_low, c;
    
    if(Serial.available() > 0)
    {
        c = Serial.read();
        
        if(c == C_REQUEST)
        {
            for(i = 0; i < APINS_N; i++)
            {
                t_int = analogRead(APINS[i]);
                t_low = t_int & 0xff;
                t_high = t_int >> 8;
                
                #ifdef DEBUG
                    Serial.print(t_int);
                    Serial.print("\t");
                #else
                    Serial.write(t_high);
                    Serial.write(t_low);
                #endif
            }
            for(i = 0; i < DPINS_N; i++)
            {
                t_int = digitalRead(DPINS[i]);
                
                //info led
                digitalWrite(infoLedPin, t_int ? HIGH: LOW);
                
                #ifdef DEBUG
                    Serial.print(t_int);
                    Serial.print("\t");
                #else
                    Serial.write(t_int);
                #endif
            }
            
            #ifdef DEBUG
                Serial.print("\n");
            #endif
        }
    }
}
