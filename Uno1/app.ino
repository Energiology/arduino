#include <Wire.h>
#include <Adafruit_INA219.h>
#include <ArduinoJson.h>

#define ARDUINO_ID 1
#define TIMEOUT 1000

// Relay
#define RY1 2
#define RY2 3
#define RY3 4
#define RY4 5
#define RY5 6
#define RY6 7
#define RY7 8
#define RY8 9
#define RY9 10
#define RY10 11

// Port
#define QC1 0; // QC1 - RY1, RY2
#define QC2 1; // QC2 - RY3, RY4
#define QC3 2; // QC3 - RY5, RY6
#define QC4 3; // QC4 - RY7, RY8
#define QC5 4; // QC5 - RY9, RY10

// Power Delivery
#define PD1 100
#define PD2 200

/**
 * Relay
 * Relay 번호 홀수는 HIGH일 시 연결, 짝수는 LOW일 시 연결
 * 즉, QC1의 공급을 PD2 -> PD1으로 바꾸고 싶다면
 * qc_relay[QC1][0] -> HIGH
 * qc_relay[QC1][1] -> HIGH
*/
int relay[11] = {0, RY1, RY2, RY3, RY4, RY5, RY6, RY7, RY8, RY9, RY10};

// qc와 연결된 릴레이, qc_relay[QC번호][0 혹은 1] = 연결된 릴레이
int qc_relay[5][2] = {{RY1, RY2}, {RY3, RY4}, {RY5, RY6}, {RY7, RY8}, {RY9, RY10}}; 

// 현재 qc와 연결된 pd 목록, qc_pd[QC번호] = 현재 연결된 QC
int qc_pd[5] = {PD2, PD2, PD2, PD2, PD2}; 

Adafruit_INA219 ina219_QC1;
Adafruit_INA219 ina219_QC2(0x41);


void setup() {
  Serial.begin(9600);
  uint32_t currentFrequency;
  ina219_QC1.begin();
  ina219_QC2.begin();

  // 각 릴레이 pinMode 초기화
  for (int i = 1; i < 11; i++) {
    pinMode(relay[i], OUTPUT);
  }
}

void loop() {

  // 명령 있을 시 수행
  if (Serial.available()) {
    // String input = Serial.readString();
    char input = Serial.read();
    Serial.print("Serial input: ");
    Serial.println(input);

    
    Serial.flush(); // 버퍼 비우기
  } 
  
  // 전류 측정
  measurePowerAllPort();
  // testOutput(); // 테스트 코드
  delay(TIMEOUT);
}

void testOutput() {
  Serial.println("Every second execute!");
}

void measurePowerAllPort() {
  float shuntvoltage_A = 0;
  float busvoltage_A = 0;
  float current_mA_A = 0;
  float loadvoltage_A = 0;
  float power_mW_A = 0;

  float shuntvoltage_B = 0;
  float busvoltage_B = 0;
  float current_mA_B = 0;
  float loadvoltage_B = 0;
  float power_mW_B = 0;

  shuntvoltage_A = ina219_QC1.getShuntVoltage_mV();       // 션트저항에 걸리는 전압 측정 (션트 전압으로 인해 드롭된 전압)
  busvoltage_A = ina219_QC1.getBusVoltage_V();            // 버스전압 - 션트전압
  current_mA_A = ina219_QC1.getCurrent_mA();              // 사용 전류
  power_mW_A = ina219_QC1.getPower_mW();                  // 사용 전력
  loadvoltage_A = busvoltage_A + (shuntvoltage_A / 1000); // 공급 전압

  shuntvoltage_B = ina219_QC2.getShuntVoltage_mV();       // 션트저항에 걸리는 전압 측정 (션트 전압으로 인해 드롭된 전압)
  busvoltage_B = ina219_QC2.getBusVoltage_V();            // 버스전압 - 션트전압
  current_mA_B = ina219_QC2.getCurrent_mA();              // 사용 전류
  power_mW_B = ina219_QC2.getPower_mW();                  // 사용 전력
  loadvoltage_B = busvoltage_B + (shuntvoltage_B / 1000); // 공급 전압

  DynamicJsonDocument powerData_A(50000);
  String jsonPowerData = "";
  // { type: 'read', 'portId, supplier, power }
    
  powerData_A["arduinoId"] = ARDUINO_ID;
  powerData_A["type"] = "read";
  powerData_A["portNum"] = 1;
  powerData_A["power"] = power_mW_A;

  serializeJson(powerData_A, jsonPowerData);

  Serial.println(jsonPowerData);

  DynamicJsonDocument powerData_B(50000);
  String jsonPowerData2 = "";
  // { type: 'read', 'portId, supplier, power }
    
  powerData_B["arduinoId"] = ARDUINO_ID;
  powerData_B["type"] = "read";
  powerData_B["portNum"] = 2;
  powerData_B["power"] = power_mW_B;

  serializeJson(powerData_B, jsonPowerData2);

  Serial.println(jsonPowerData2);
}

/**
 * QC와 연결된 PD를 변경해주는 함수
 * @param QC {int} - PD를 변경할 타겟 QC
 * @param PD {int} - 변경할 타겟 PD ex) PD1, PD2
*/
void switchPD(int QC, int PD) {
  int value;
  
  if (PD == PD1) {
    value = HIGH;
  } else if (PD == PD2) {
    value = LOW;
  }

  // 릴레이 제어
  for (int i=0; i<2; i++) {
    digitalWrite(qc_relay[QC][i], value);
  }

  qc_pd[QC] = PD; // 현재 QC와 연결된 PD가 무엇인지 변경
}