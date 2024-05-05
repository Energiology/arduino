#include <ArduinoJson.h>

#define ARDUINO_ID 1
#define TIMEOUT 1000


void setup() {
  Serial.begin(9600);
}

void loop() {
  measurePowerAllPort();
  
  delay(TIMEOUT);
}

void measurePowerAllPort() {
  DynamicJsonDocument powerData_A(50000);
  String jsonPowerData = "";
  // { type: 'read', 'portId, supplier, power }
    
  powerData_A["arduinoId"] = ARDUINO_ID;
  powerData_A["type"] = "read";
  powerData_A["portNum"] = 1;
  powerData_A["power"] = 20000;

  serializeJson(powerData_A, jsonPowerData);

  Serial.print(jsonPowerData);

  DynamicJsonDocument powerData_B(50000);
  String jsonPowerData2 = "";
    
  powerData_B["arduinoId"] = ARDUINO_ID;
  powerData_B["type"] = "read";
  powerData_B["portNum"] = 2;
  powerData_B["power"] = 30000;

  serializeJson(powerData_B, jsonPowerData2);

  Serial.print(jsonPowerData2);
}


