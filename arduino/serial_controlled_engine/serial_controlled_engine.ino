const int motorPin = 13;
int speed = 100;

void setSpeed()
{
    analogWrite(motorPin, speed);
}

void setup()
{
    Serial.begin(115200);
    pinMode(motorPin, OUTPUT);
    Serial.setTimeout(15);
    setSpeed();
}

void loop()
{
    int t_speed;
    if(Serial.available() > 0)
    {
        t_speed = Serial.parseInt();
        if(t_speed >= 100 && t_speed <= 254)
        {
            Serial.print("Setting speed to ");
            Serial.println(t_speed);
            speed = t_speed;
        }
        else
            Serial.println("Range is [100...254]!");
        setSpeed();
    }
}
