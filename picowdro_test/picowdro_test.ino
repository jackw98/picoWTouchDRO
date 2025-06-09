#include <SerialBT.h>

#define LED_PIN LED_BUILTIN

unsigned long lastSend = 0;
const unsigned long dataInterval = 500;  // 0.5 seconds = 1 Hz data update
const unsigned long led1HzInterval = 500;
const unsigned long led5HzInterval = 100;

bool ledState = false;
unsigned long lastLedToggle = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  SerialBT.setName("PicoW DRO Test");
  SerialBT.begin();
}

void loop() {
  unsigned long now = millis();

  // Bluetooth connected: send data and blink at 1Hz
  if (SerialBT) {
    if (now - lastSend >= dataInterval) {
      lastSend = now;

      // Generate test data
      int w = random(-9999, 10000);
      int x = random(-9999, 10000);
      int y = random(-9999, 10000);
      int z = random(-9999, 10000);
      int t = random(-9999, 10000);

      char buf[128];
      snprintf(buf, sizeof(buf), "w%d;x%d;y%d;z%d;t%d;\r\n", w, x, y, z, t);
      SerialBT.print(buf);
    }

    // 1Hz LED blink (on 0.5s, off 0.5s)
    if (now - lastLedToggle >= led1HzInterval) {
      lastLedToggle = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  } 
  else {
    // 5Hz LED blink (on/off every 100ms)
    if (now - lastLedToggle >= led5HzInterval) {
      lastLedToggle = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }

  // Discard incoming data if any
  while (SerialBT.available()) {
    SerialBT.read();
  }
}
