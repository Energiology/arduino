#include "power_measure.h"
#include <Adafruit_INA219.h>
#include <ArduinoJson.h>
const int ARDUINO_ID = 1;

const int CURRENCY_MEASURE_LENGTH = 3;
Adafruit_INA219 ina219_QC[3] = {
  Adafruit_INA219(0x44),
  Adafruit_INA219(0x41),
  Adafruit_INA219(0x40)
};

/**
 * INA219 설정 초기화
*/
void initializeINA219() {
  for (int i = 0; i< CURRENCY_MEASURE_LENGTH; i++) {
    ina219_QC[i].begin();
  }
};

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
