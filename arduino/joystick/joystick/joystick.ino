
int X_SensorPin = A0;   
int Y_SensorPin = A1;   

int ledPin = 13;

int X = 0;
int Y = 0;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  X = analogRead(X_SensorPin);
  Y = analogRead(Y_SensorPin);

  Serial.print("(");
  Serial.print(X);
  Serial.print("; ");
  Serial.print(Y);
  Serial.println(")");
  
  delay(100);
}

