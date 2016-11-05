// Receiver

#include <VirtualWire.h>

const int LED_PIN = 2;
const int SPK_PIN = 3;
const int RX_PIN = 12;
const int POWER_PIN = 11;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(SPK_PIN, OUTPUT);
  pinMode(RX_PIN, INPUT);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
  
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(RX_PIN);
  vw_setup(4000);
  vw_rx_start();
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen))
    {
      if(buf[0]=='A') {  
        digitalWrite(LED_PIN, HIGH);
        tone(SPK_PIN, 1000, 1000);
      }  else {
        digitalWrite(LED_PIN, LOW);
        tone(SPK_PIN, 500, 1000);
      }
    }
}
