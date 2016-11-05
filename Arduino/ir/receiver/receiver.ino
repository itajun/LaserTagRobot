// IR

const int RECEIVE_PIN = A0;
const int SPK_PIN = 13;

unsigned short minValue = 1023;
unsigned short maxValue = 0;

void setup() {  
    pinMode(RECEIVE_PIN, INPUT);  
    pinMode(SPK_PIN, OUTPUT);
    Serial.begin(9600);    
}  

void loop() {  
    unsigned short value = analogRead(RECEIVE_PIN);
    if (minValue > value) minValue = value;
    if (maxValue < value) maxValue = value;
    Serial.print(value);
    Serial.print(" ");
    Serial.print(minValue);
    Serial.print(" ");
    Serial.print(maxValue);
    Serial.print(" ");
    value = map(value, minValue, maxValue, 0, 10);
    Serial.println(value);
    if (value < 9) {
      tone(SPK_PIN, 500, 500);
      delay(1000);    
    } else {
      delay(20);
    }
}  
