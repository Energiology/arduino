#include <Wire.h>
#include "power_measure.h"
#include "power_control.h"


const int TIMEOUT = 1000;

const char COMMAND_QC1_PD1 = 'a';
const char COMMAND_QC1_PD2 = 'b';
const char COMMAND_QC1_OFF = 'c';

const char COMMAND_QC2_PD1 = 'd';
const char COMMAND_QC2_PD2 = 'e';
const char COMMAND_QC2_OFF = 'f';

const char COMMAND_QC3_PD1 = 'g';
const char COMMAND_QC3_PD2 = 'h';
const char COMMAND_QC3_OFF = 'i';

const char COMMAND_QC4_PD1 = 'j';
const char COMMAND_QC4_PD2 = 'k';
const char COMMAND_QC4_OFF = 'l';

const char COMMAND_QC5_PD1 = 'm';
const char COMMAND_QC5_PD2 = 'n';
const char COMMAND_QC5_OFF = 'o';






void setup() {
  Serial.begin(9600);

  initializeINA219(); // ina219 설정
  delay(1000);

  // 연결된 PD 초기화
  initializePD();
  delay(1000);
}

void loop() {
  // 명령 있을 시 수행
  if (Serial.available()) {
    // String input = Serial.readString();
    char input = Serial.read();
    Serial.print("Serial input: ");
    Serial.println(input);

    switch (input) {
      case COMMAND_QC1_PD1:
        switchPD(QC1, PD_1);
        break;
      case COMMAND_QC1_PD2:
        switchPD(QC1, PD_2);
        break;
      case COMMAND_QC1_OFF:
        turnOffPD(QC1);
        break;

      case COMMAND_QC2_PD1:
        switchPD(QC2, PD_1);
        break;
      case COMMAND_QC2_PD2:
        switchPD(QC2, PD_2);
        break;
      case COMMAND_QC2_OFF:
        turnOffPD(QC2);
        break;

      case COMMAND_QC3_PD1:
        switchPD(QC3, PD_1);
        break;
      case COMMAND_QC3_PD2:
        switchPD(QC3, PD_2);
        break;
      case COMMAND_QC3_OFF:
        turnOffPD(QC3);
        break;

      case COMMAND_QC4_PD1:
        switchPD(QC4, PD_1);
        break;
      case COMMAND_QC4_PD2:
        switchPD(QC4, PD_2);
        break;
      case COMMAND_QC4_OFF:
        turnOffPD(QC4);
        break;

      case COMMAND_QC5_PD1:
        switchPD(QC5, PD_1);
        break;
      case COMMAND_QC5_PD2:
        switchPD(QC5, PD_2);
        break;
      case COMMAND_QC5_OFF:
        turnOffPD(QC5);
        break;

      default:
        break;
    }

    while (Serial.available()) {
      Serial.flush(); // 버퍼 비우기
    }
  } 
  
  // 전류 측정
  measurePowerAllPort();

  delay(TIMEOUT);
}

