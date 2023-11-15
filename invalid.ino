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

  // Split commands by commas
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
