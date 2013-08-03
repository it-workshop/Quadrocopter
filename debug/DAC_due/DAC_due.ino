#define PIN_IN A0
#define PIN_OUT DAC1

int t;

void setup()
{
    pinMode(PIN_IN, INPUT);
    pinMode(PIN_OUT, OUTPUT);
    Serial.begin(115200);
    analogWriteResolution(12);
}

void loop()
{
    t = analogRead(PIN_IN);
    t = map(t, 0, 1023, 0, 4095);
    analogWrite(PIN_OUT, t);
    Serial.println(t);
    delay(10);
}
