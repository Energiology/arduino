#include <Wire.h>
#include "power_measure.h"

const int TIMEOUT = 1000;

void setup() {
  Serial.begin(9600);

  initializeINA219(); // ina219 설정
  delay(1000);
}

void loop() {
  // 전류 측정
  measurePowerAllPort();

  delay(TIMEOUT);
}

