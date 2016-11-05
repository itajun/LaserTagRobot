// IR

const int EMIT_PIN = 2;
const int LED_PIN = 13;

unsigned long lastChange = 0;
  
void setup() {  
    pinMode(EMIT_PIN, OUTPUT);  
    pinMode(LED_PIN, OUTPUT);  
}  

void loop() {  
    if (millis() - lastChange > 1000) {
        lastChange = millis();
        if ((lastChange / 1000) % 2 == 0) {
          digitalWrite(EMIT_PIN, HIGH);
          digitalWrite(LED_PIN, HIGH);
        } else {
          digitalWrite(EMIT_PIN, LOW);
          digitalWrite(LED_PIN, LOW);
        }
    }
}  
