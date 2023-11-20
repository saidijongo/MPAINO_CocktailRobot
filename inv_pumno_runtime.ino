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

  int firstBracketIndex = command.indexOf('(');
  int secondBracketIndex = command.indexOf(')', firstBracketIndex + 1);

  if (firstBracketIndex != -1 && secondBracketIndex != -1) {
    // Extract motor type and SID from the first set of brackets
    String motorTypeAndSID = command.substring(firstBracketIndex + 1, secondBracketIndex);
    int spaceIndex = motorTypeAndSID.indexOf(' ');

    if (spaceIndex != -1) {
      String motorType = motorTypeAndSID.substring(0, spaceIndex);
      String SID = motorTypeAndSID.substring(spaceIndex + 1);

      Serial.print("Motor Type: ");
      Serial.println(motorType);
      Serial.print("SID: ");
      Serial.println(SID);
    }

    int index = secondBracketIndex + 1;

    while (index < command.length()) {
      int endIndex = command.indexOf(')', index);

      if (endIndex != -1) {
        // Extract pump number and run time from subsequent brackets
        String pumpData = command.substring(index + 1, endIndex);

        int commaIndex = pumpData.indexOf(separator);
        if (commaIndex != -1) {
          int pumpNumber = pumpData.substring(0, commaIndex).toInt();
          int runTime = pumpData.substring(commaIndex + 1).toInt();

          if (pumpNumber >= 54 && pumpNumber <= 85) {
            activateMotor(motorPins[pumpNumber - 54]);
            pumpStartTimes[pumpNumber - 54] = millis() + runTime; // Set the time when the pump should be turned off
            Serial.print("Mixing pump: ");
            Serial.println(pumpNumber);
          } else {
            Serial.println("Invalid pump number");
          }
        } else {
          Serial.println("Invalid pump data format");
        }

        index = endIndex + 1;
      } else {
        break;
      }
    }
  } else {
    Serial.println("Invalid command format");
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
