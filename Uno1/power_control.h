#ifndef POWER_CONTROL_H
#define POWER_CONTROL_H

#include <Arduino.h>

// Relay
const int RY1 = 2;
const int RY2 = 3;
const int RY3 = 4;
const int RY4 = 5;
const int RY5 = 6;
const int RY6 = 7;
const int RY7 = 8;
const int RY8 = 9;
const int RY9 = 10;
const int RY10 = 11;

// Port
const int QC1 = 0; // QC1 - RY1, RY2
const int QC2 = 1; // QC2 - RY3, RY4
const int QC3 = 2; // QC3 - RY5, RY6
const int QC4 = 3; // QC4 - RY7, RY8
const int QC5 = 4; // QC5 - RY9, RY10

// Power Delivery
const int PD_1 = 0;
const int PD_2 = 1;
const int PD_OFF = 2;

void initializePD();
void switchPD(int QC, int targetPD);
void turnOffPD(int QC);
void connectRelay(int relayNum);
void disconnectRelay(int relayNum);

#endif
