//"(PUMPMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(54,1000),(60,1500),(80,2000),(58,1000),(70,1500)"

#include <Arduino.h>

//const int motorPins[] = {54, 55, 56, 57, 58, 59};
const int motorPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};
const int dirPin = 13;
const int pulPin = 12;
const int servoPWM = 11;

const int numPumps = sizeof(motorPins) / sizeof(motorPins[0]);

unsigned long pumpStartTimes[numPumps] = {0};

void setup() {
  for (int i = 0; i < numPumps; i++) {
    pinMode(motorPins[i], OUTPUT);
    digitalWrite(motorPins[i], LOW);
  }

  pinMode(dirPin, OUTPUT);
  pinMode(pulPin, OUTPUT);
  pinMode(servoPWM, OUTPUT);

  Serial.begin(115200);
}

void runPumps(int pumpNumber, int runTime) {
  digitalWrite(motorPins[pumpNumber - 54], HIGH);
  pumpStartTimes[pumpNumber - 54] = millis() + runTime; // Set the time when the pump should be turned off
  Serial.print("Running pump: ");
  Serial.println(pumpNumber);
}

void runServo(int angle, int runTime) {
  // Move the servo to the specified angle
  analogWrite(servoPWM, angle);

  // Wait for the specified time
  delay(runTime);
}

void runStepper(int angle, int runTime) {
  int direction = (angle >= 0) ? HIGH : LOW;
  angle = abs(angle);
  digitalWrite(dirPin, direction);

  // Calculate the number of steps based on the angle
  int steps = angle * 6400 / 360; // Assuming a 200 steps per revolution stepper motor

  // Run the stepper motor
  for (int i = 0; i < steps; i++) {
    digitalWrite(pulPin, HIGH);
    delayMicroseconds(500); // Adjust this delay as needed for your motor
    digitalWrite(pulPin, LOW);
    delayMicroseconds(500);
  }

  // Run for the requested time
  delay(runTime);
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

      // Extract pump number/angle and run time from subsequent brackets
      int index = secondBracketIndex + 1;
      while (index < command.length()) {
        int nextBracketIndex = command.indexOf('(', index);
        int endIndex = command.indexOf(')', nextBracketIndex + 1);

        if (nextBracketIndex != -1 && endIndex != -1) {
          String inputData = command.substring(nextBracketIndex + 1, endIndex);

          int commaIndex = inputData.indexOf(separator);
          if (commaIndex != -1) {
            int param1 = inputData.substring(0, commaIndex).toInt();
            int param2 = inputData.substring(commaIndex + 1).toInt();

            if (motorType == "PUMPMOTOR_OPERATION")
              runPumps(param1, param2);
            else if (motorType == "SERVOMOTOR_OPERATION")
              runServo(param1, param2);
            else if (motorType == "STEPPERMOTOR_OPERATION")
              runStepper(param1, param2);
            else
              Serial.println("Unknown motor type");
          } else {
            Serial.println("Invalid pump data format");
          }

          index = endIndex + 1;
        } else {
          break;
        }
      }
    } else {
      Serial.println("Invalid motor type and SID format");
    }
  } else {
    Serial.println("Invalid command format");
  }
}

void loop() {
  // Check and deactivate pumps if the run time has elapsed
  for (int i = 0; i < numPumps; i++) {
    if (millis() >= pumpStartTimes[i] && digitalRead(motorPins[i]) == HIGH) {
      digitalWrite(motorPins[i], LOW);
    }
  }

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    processCommand(data);
  }
}
// "(PUMPMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(56,3000),(58,4250),(56,3000),(60,4000)"
//"(PUMPMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(54,1000),(55,425--2000),(56,3000),(57,4000),(58,3000)"
//"(SERVOMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(90,3000)"
//"(STEPPERMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(360,2000)"
//"(STEPPERMOTOR_OPERATION 1647eba3-a6b0-42a7-8a08-ffef8ab07065),(-360,2000)"
