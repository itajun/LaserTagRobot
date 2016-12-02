void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  digitalWrite(A3, random(100, 255));
  digitalWrite(A4, random(100, 255));
  digitalWrite(A5, random(100, 255));
  delay(5000);

  analogWrite(8, random(150, 255));
  digitalWrite(9, LOW);
  analogWrite(10, random(150, 255));
  digitalWrite(11, LOW);
}
