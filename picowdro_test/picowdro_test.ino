#include <SerialBT.h>
#include "pio_encoder.h"

#define LED_PIN LED_BUILTIN

// Encoders A, B, C (X, Y, Z)
PioEncoder encA(2);
PioEncoder encB(6);
PioEncoder encC(8);

unsigned long lastSend = 0;
const unsigned long dataInterval = 10;      // 10 ms
const unsigned long ledPulseDuration = 50;  // LED on time after sending

bool ledOn = false;
unsigned long ledTurnOffTime = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  encA.begin();
  encB.begin();
  encC.begin();

  SerialBT.setName("PicoW DRO");
  SerialBT.begin();
}

void loop() {
  unsigned long now = millis();

  // Send encoder data every 10ms
  if (SerialBT && now - lastSend >= dataInterval) {
    lastSend = now;

    int x = encA.getCount();
    int y = encB.getCount();
    int z = encC.getCount();

    char buf[64];
    snprintf(buf, sizeof(buf), "x%d;y%d;z%d;\r\n", x, y, z);
    SerialBT.print(buf);

    // Start non-blocking LED blink
    digitalWrite(LED_PIN, HIGH);
    ledOn = true;
    ledTurnOffTime = now + ledPulseDuration;
  }

  // Turn off LED if pulse duration has elapsed
  if (ledOn && millis() >= ledTurnOffTime) {
    digitalWrite(LED_PIN, LOW);
    ledOn = false;
  }

  // Discard incoming BT data
  while (SerialBT.available()) {
    SerialBT.read();
  }
}
