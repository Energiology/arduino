/**
 * Relay 번호 홀수는 HIGH일 시 연결, 짝수는 LOW일 시 연결
 * qc_relay[QC][0] -> HIGH = 연결 / LOW = 해제
 * qc_relay[QC][1] -> HIGH = 해제 / LOW = 연결
 * 
 * 즉, QC1의 PD를 PD_2 -> PD_1으로 바꾸고 싶다면
 * qc_relay[QC1][1] -> HIGH = 해제
 * qc_relay[QC1][0] -> HIGH = 연결
*/

#include "power_control.h"

const int relay[11] = {-1, RY1, RY2, RY3, RY4, RY5, RY6, RY7, RY8, RY9, RY10};
const int qc_relay[5][2] = {{RY1, RY2}, {RY3, RY4}, {RY5, RY6}, {RY7, RY8}, {RY9, RY10}}; // qc와 연결된 릴레이, qc_relay[QC번호][0 혹은 1] = 연결된 릴레이

// 현재 qc와 연결된 pd 목록, qc_pd[QC번호] = 현재 연결된 QC
int qc_pd[5] = {PD_2, PD_2, PD_2, PD_2, PD_2}; 

/**
 * 연결된 qc와 pd를 초기화
*/
void initializePD() {
  // 각 릴레이 pinMode 초기화
  for (int i = 1; i < 11; i++) {
    pinMode(relay[i], OUTPUT);
  }

  delay(1000);

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