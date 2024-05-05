#ifndef POWER_MEASUREMENT_H
#define POWER_MEASUREMENT_H

#include <Arduino.h>

void initializeINA219();
void measurePowerAllPort();
void measurePower(int qc);

#endif
