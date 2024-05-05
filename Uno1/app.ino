/**
 * Relay 번호 홀수는 HIGH일 시 연결, 짝수는 LOW일 시 연결
 * qc_relay[QC][0] -> HIGH = 연결 / LOW = 해제
 * qc_relay[QC][1] -> HIGH = 해제 / LOW = 연결
 * 
 * 즉, QC1의 PD를 PD_2 -> PD_1으로 바꾸고 싶다면
 * qc_relay[QC1][1] -> HIGH = 해제
 * qc_relay[QC1][0] -> HIGH = 연결
*/

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <ArduinoJson.h>

const int ARDUINO_ID = 1;
const int TIMEOUT = 1000;

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

const int relay[11] = {-1, RY1, RY2, RY3, RY4, RY5, RY6, RY7, RY8, RY9, RY10};
const int qc_relay[5][2] = {{RY1, RY2}, {RY3, RY4}, {RY5, RY6}, {RY7, RY8}, {RY9, RY10}}; // qc와 연결된 릴레이, qc_relay[QC번호][0 혹은 1] = 연결된 릴레이

// 현재 qc와 연결된 pd 목록, qc_pd[QC번호] = 현재 연결된 QC
int qc_pd[5] = {PD_2, PD_2, PD_2, PD_2, PD_2}; 

const int CURRENCY_MEASURE_LENGTH = 3;
Adafruit_INA219 ina219_QC[3] = {
  Adafruit_INA219(0x44),
  Adafruit_INA219(0x41),
  Adafruit_INA219(0x40)
};

void setup() {
  Serial.begin(9600);

  // ina219 설정
  for (int i = 0; i< CURRENCY_MEASURE_LENGTH; i++) {
    ina219_QC[i].begin();
  }
  delay(1000);

  // 각 릴레이 pinMode 초기화
  for (int i = 1; i < 11; i++) {
    pinMode(relay[i], OUTPUT);
  }
  delay(1000);

  // 연결된 PD 초기화
  initializePD();
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


/**
 * 전체 포트에 연결된 전력 측정
*/
void measurePowerAllPort() {
  for (int qc = 0; qc < CURRENCY_MEASURE_LENGTH; qc++) {
    measurePower(qc);
  }
}

/**
 * 특정 포트에 측정된 전력 출력
 * @param qc {int} - QC(port) 번호. 번호는 0부터 시작한다.
*/
void measurePower(int qc) {
  float power_mW = 0;

  power_mW = ina219_QC[qc].getPower_mW();

  DynamicJsonDocument powerData(50000);
  String jsonPowerData = "";
    
  powerData["arduinoId"] = ARDUINO_ID;
  powerData["type"] = "read";
  powerData["portNum"] = qc + 1;
  powerData["power"] = power_mW;

  serializeJson(powerData, jsonPowerData);

  Serial.println(jsonPowerData);
}

/**
 * 연결된 qc와 pd를 초기화
*/
void initializePD() {
  for(int qcNum = 0; qcNum < 5; qcNum++) {
    switchPD(qcNum, PD_2); // PD_2로 연결 초기화
  }
}

/**
 * QC와 연결된 PD를 변경해주는 함수
 * @param QC {int} - PD를 변경할 QC
 * @param targetPD {int} - 변경할 타겟 PD ex) PD_1, PD_2
*/
void switchPD(int QC, int targetPD) {
  // 릴레이 제어
  switch (targetPD) {
    case PD_1:
      disconnectRelay(qc_relay[QC][PD_2]); // PD_2 연결 해제
      delay(100);
      connectRelay(qc_relay[QC][PD_1]); // PD_1 연결
      break;

    case PD_2:
      disconnectRelay(qc_relay[QC][PD_1]); // PD_1 연결 해제 
      delay(100);
      connectRelay(qc_relay[QC][PD_2]); // PD_2 연결
      break;

    default:
      Serial.println("Switch PD Error!");
      return;
  }

  qc_pd[QC] = targetPD; // 현재 QC와 연결된 PD가 무엇인지 변경
}

/**
 * 해당 QC의 연결된 전원 차단(전원 off)
 * @param {int} - 전원을 차단할 QC
*/
void turnOffPD(int QC) {
  disconnectRelay(qc_relay[QC][PD_1]);
  disconnectRelay(qc_relay[QC][PD_2]);
  
  qc_pd[QC] = PD_OFF; // 현재 QC와 연결된 PD는 없음
}

/**
 * 릴레이 연결 함수
 * @param relayNum {int} - 연결할 릴레이
*/
void connectRelay(int relayNum) {
  switch (relayNum) {
    case RY1:
    case RY3:
    case RY5:
    case RY7:
    case RY9:
      digitalWrite(relayNum, HIGH);
      break;
    case RY2:
    case RY4:
    case RY6:
    case RY8:
    case RY10:
      digitalWrite(relayNum, LOW);
      break;
    default:
      Serial.println("Connect Relay Error!");
      break;
  }
}

/**
 * 릴레이 연결 해제 함수
 * @param relayNum {int} - 연결 해제할 릴레이
*/
void disconnectRelay(int relayNum) {
  switch (relayNum) {
    case RY1:
    case RY3:
    case RY5:
    case RY7:
    case RY9:
      digitalWrite(relayNum, LOW);
      break;
    case RY2:
    case RY4:
    case RY6:
    case RY8:
    case RY10:
      digitalWrite(relayNum, HIGH);
      break;
    default:
      Serial.println("Disconnect Relay Error!");
      break;
  }
}