#include <Wire.h>
#include <Adafruit_INA219.h>
#include <ArduinoJson.h>

#define ARDUINO_ID 1
#define TIMEOUT 1000

Adafruit_INA219 ina219_A;
Adafruit_INA219 ina219_B(0x41);

int i = 0;

void setup() {
  Serial.begin(9600);
  uint32_t currentFrequency;
  ina219_A.begin();
  ina219_B.begin();
}

void loop() {
  
  measurePowerAllPort();
  

  delay(TIMEOUT);
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

  shuntvoltage_A = ina219_A.getShuntVoltage_mV();       // 션트저항에 걸리는 전압 측정 (션트 전압으로 인해 드롭된 전압)
  busvoltage_A = ina219_A.getBusVoltage_V();            // 버스전압 - 션트전압
  current_mA_A = ina219_A.getCurrent_mA();              // 사용 전류
  power_mW_A = ina219_A.getPower_mW();                  // 사용 전력
  loadvoltage_A = busvoltage_A + (shuntvoltage_A / 1000); // 공급 전압

  shuntvoltage_B = ina219_B.getShuntVoltage_mV();       // 션트저항에 걸리는 전압 측정 (션트 전압으로 인해 드롭된 전압)
  busvoltage_B = ina219_B.getBusVoltage_V();            // 버스전압 - 션트전압
  current_mA_B = ina219_B.getCurrent_mA();              // 사용 전류
  power_mW_B = ina219_B.getPower_mW();                  // 사용 전력
  loadvoltage_B = busvoltage_B + (shuntvoltage_B / 1000); // 공급 전압

  DynamicJsonDocument powerData_A(50000);
  String jsonPowerData = "";
  // { type: 'read', 'portId, supplier, power }
    
  powerData_A["arduinoId"] = ARDUINO_ID;
  powerData_A["type"] = "read";
  powerData_A["portNum"] = 1;
  powerData_A["power"] = power_mW_A;

  serializeJson(powerData_A, jsonPowerData);

  Serial.print(jsonPowerData);

  DynamicJsonDocument powerData_B(50000);
  String jsonPowerData2 = "";
  // { type: 'read', 'portId, supplier, power }
    
  powerData_B["arduinoId"] = ARDUINO_ID;
  powerData_B["type"] = "read";
  powerData_B["portNum"] = 2;
  powerData_B["power"] = power_mW_B;

  serializeJson(powerData_B, jsonPowerData2);

  Serial.print(jsonPowerData2);
}


// #include <ArduinoJson.h>

// #define PORT_LENGTH 3
// #define BATTERY 'B'
// #define EXTERNAL_SUPPLIER 'E'

// #define MEASURE_POWER_PER_SECOND "measure"
// #define SWITCH_POWER_SUPPLIER "switch"



// signed char supplier[PORT_LENGTH] = {EXTERNAL_SUPPLIER, EXTERNAL_SUPPLIER, EXTERNAL_SUPPLIER};
// int portId[PORT_LENGTH] = {0, 1, 2};

// void setup()
// {
// 	Serial.begin(9600);
//   while(Serial.available()) continue;
// }

// void loop()
// {
//   if(Serial.available()) {
//     String json = Serial.readString();
//     // String json = receiveRequest();

//     "M 1 3" // 3번포트를 1번 공급원으로 변경해라

//     // 60초동안 59개, 58개     20개의 평균 * (60/20) = 1분 사용 전력량
    
//     const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3);
//     DynamicJsonDocument doc(capacity);

//     DeserializationError error = deserializeJson(doc, json);

//     if (error) {
//       Serial.print(F("deserializeJson() failed: "));
//       Serial.println(error.f_str());
//       return;
//     }

//     const char* command = doc["method"];

//     // Serial.println(command);

//     if (strcmp(command, MEASURE_POWER_PER_SECOND) == 0) {
//       sendPowerPerSecond();
//     } else if (strcmp(command, SWITCH_POWER_SUPPLIER) == 0) {
//       int portId = doc["payload"]["portId"];
//       const char* target = doc["payload"]["target"];

//       sendSwitchPowerSupplier(1, BATTERY);
//     } else {
//       Serial.println("Command Error!");
//     }
//   }
// }

// float measureCurrent(int portId) {
//   // 실제 출력 포트의 전류를 측정하는 코드 삽입 필요

//   return (float)portId;
// };

// float measureVoltage(int portId) {
//   // 실제 공급원의 전압을 측정하는 코드 삽입 필요

//   return (float)portId;
// };

// float measurePower(int portId) {
//   float current = measureCurrent(portId);
//   float voltage = measureVoltage(portId);
  
//   return current * voltage / 3600;
// };

// String measurePowerEveryPort() {
//   DynamicJsonDocument resultJson(5000);
//   DynamicJsonDocument arrayDoc(5000);
//   JsonArray array = arrayDoc.to<JsonArray>();
//   String powerJson = "";

//   // 각 포트별 사용 전력량 측정
//   for (int i = 0; i < PORT_LENGTH; i++) {
//     DynamicJsonDocument powerData(500);
    
//     powerData["portNumber"] = portId[i];
//     powerData["powerSupplier"] = supplier[i];
//     powerData["powerUsage"] = measurePower(portId[i]);

//     array.add(powerData);
//   }

  
//   serializeJson(array, powerJson);

//   return powerJson;
// }

// void sendPowerPerSecond() {
//   DynamicJsonDocument doc(200);

//   String data = "";
//   String powerJson = measurePowerEveryPort();

//   doc["method"] = "measure";
//   doc["payload"] = powerJson;

//   serializeJson(doc, data);
//   Serial.println(data);
// };

// String switchPowerSupplier(int portId, char target) {
//   return "Yes!";
// }

// void sendSwitchPowerSupplier(int portId, char target) {
//   String result = switchPowerSupplier(portId, target);
//   Serial.println(result);
// }

// String receiveRequest() {
//   return Serial.readString();
// }