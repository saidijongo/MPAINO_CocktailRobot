#include <Arduino.h>

const int motorPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};
const int numPumps = sizeof(motorPins) / sizeof(motorPins[0]);
const int FLOW_RATE = 1.5; // in ml/minute

unsigned long pumpStartTimes[numPumps] = {0};

void activateMotor(int pin) {
  digitalWrite(pin, HIGH);
}

void deactivateMotor(int pin) {
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
        activateMotor(motorPins[pumpNumber - 54]);
        pumpStartTimes[pumpNumber - 54] = millis() + runTime; // Set the time when the pump should be turned off
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
  for (int i = 0; i < numPumps; i++) {
    pinMode(motorPins[i], OUTPUT);
  }

  Serial.begin(115200);
}

void loop() {
  for (int i = 0; i < numPumps; i++) {
    if (millis() >= pumpStartTimes[i] && digitalRead(motorPins[i]) == HIGH) {
      deactivateMotor(motorPins[i]); // Turn off the pump when the run time has elapsed
    }
  }

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    processCommand(data);
  }
}
