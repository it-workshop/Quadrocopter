int led = 13;
int sensor = A0;
 
void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);     
}
 
void loop() {
  int Value = analogRead(A0);
  int sensorValue = map(Value, 0, 1024, 0 , 200);
    analogWrite(13, sensorValue);

  Serial.println(sensorValue);
  delay(100);
}
