void setup()
{
    delay(1000);
    
    Serial.begin(38400);
    
    pinMode(13, OUTPUT);
    
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    
    delay(5000);
}

void loop()
{
    char c;
    int i;
    
    digitalWrite(13, LOW);
    
    //It works
    Serial.print("AT+UART=115200,0,0\r\n"); //\r\n

    //Serial.print("AT+ORGL\r\n"); //\r\n
    
    delay(50);
    
    while(Serial.available() > 0)
    {
        c = Serial.read();
        
        for(i = 7; i >= 0; i--)
        {
            digitalWrite(13, HIGH);
            delay(100);
            digitalWrite(13, LOW);
            delay(100);
            digitalWrite(13, HIGH);
            delay(100);
            digitalWrite(13, LOW);
            delay(100);
            digitalWrite(13, (c & (1 << i)) ? HIGH: LOW);
            delay(1000);
        }
    }
    
    for(; ; );
}
