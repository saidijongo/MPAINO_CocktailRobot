#include <Arduino.h>

const int motorPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};
const int FLOW_RATE = 1.5; // in ml/minute

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void activateMotor(int pin, int runTime) {
  digitalWrite(pin, HIGH);
  delay(runTime);
  digitalWrite(pin, LOW);
}

void runPumps(String commandStr) {
  commandStr.trim();

  // Remove leading and trailing parentheses
  commandStr = commandStr.substring(1, commandStr.length() - 1);

  // Process the entire command string
  int commaIndex = commandStr.indexOf(',');
  while (commaIndex != -1) {
    String command = commandStr.substring(0, commaIndex);
    commandStr = commandStr.substring(commaIndex + 1);

    int pumpNumber = getValue(command, ',', 0).toInt();
    int runTime = getValue(command, ',', 1).toInt();

    if (pumpNumber >= 54 && pumpNumber <= 85) {
      activateMotor(motorPins[pumpNumber - 54], runTime);
      Serial.print("Mixing pump: ");
      Serial.println(pumpNumber);
    } else {
      Serial.println("Invalid pump number");
    }

    commaIndex = commandStr.indexOf(',');
  }
}

void setup() {
  for (int i = 0; i < sizeof(motorPins) / sizeof(motorPins[0]); i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    runPumps(data);
  }
}
//"(54,10000),(55,11000),(56,12000),(57,13000),(58,14000),(59,15000),(60,16000),(61,17000),(62,18000),(63,19000),(64,20000),(65,10000),(66,11000),(67,12000),(68,13000),(69,14000),(70,15000),(71,16000),(72,17000),(73,18000),(74,19000),(75,20000),(76,10000),(77,11000),(78,12000),(79,13000),(80,14000),(81,15000),(82,16000),(83,17000),(84,18000),(85,19000)"
