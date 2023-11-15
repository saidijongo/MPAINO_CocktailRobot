#include <Arduino.h>

const int motorPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};
const int FLOW_RATE = 1.5; // in ml/minute

void activateMotor(int pin, int runTime) {
  Serial.print("Activating motor on pin ");
  Serial.println(pin);

  digitalWrite(pin, HIGH);
  delay(runTime);
  digitalWrite(pin, LOW);
}

void processCommand(String command) {
  char separator = ',';

  int index = command.indexOf('(');

  while (index != -1) {
    int endIndex = command.indexOf(')', index + 1);

    if (endIndex != -1) {
      String pumpData = command.substring(index + 1, endIndex);
      int pumpNumber = pumpData.substring(0, pumpData.indexOf(separator)).toInt();
      int runTime = pumpData.substring(pumpData.indexOf(separator) + 1).toInt();

      if (pumpNumber >= 54 && pumpNumber <= 85) {
        activateMotor(motorPins[pumpNumber - 54], runTime);
        Serial.print("Mixing pump: ");
        Serial.println(pumpNumber);
      } else {
        Serial.println("Invalid pump number");
      }

      index = command.indexOf('(', endIndex + 1);
    } else {
      break;
    }
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
    processCommand(data);
  }
}
