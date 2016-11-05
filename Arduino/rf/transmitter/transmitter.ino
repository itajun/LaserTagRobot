// Transmit

#include <VirtualWire.h>

const int LED_PIN = 2;
const int TX_PIN = 12;
const int POWER_PIN = 13;

char *outBuffer[2];

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TX_PIN, OUTPUT);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(TX_PIN);
  vw_setup(4000);
}

void loop(){
  vw_send((uint8_t *) "A", 1);
  vw_wait_tx();
  digitalWrite(LED_PIN,HIGH);
  delay(2000);
  
  vw_send((uint8_t *) "B", 1);
  vw_wait_tx();
  digitalWrite(LED_PIN,LOW);
  delay(2000);
}
