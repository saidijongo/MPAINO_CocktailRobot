#include <Arduino.h>

const int motorPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};
const int FLOW_RATE = 1.5; // in ml/minute

void activateMotor(int pin, int runTime) {
  digitalWrite(pin, HIGH);
  delay(runTime);
  digitalWrite(pin, LOW);
}

String getValue(String data, char separator, int outerIndex, int innerIndex) {
  int outerFound = 0;
  int outerStrIndex[] = {0, -1};
  int maxOuterIndex = data.length() - 1;

  for (int i = 0; i <= maxOuterIndex && outerFound <= outerIndex; i++) {
    if (data.charAt(i) == separator || i == maxOuterIndex) {
      outerFound++;
      outerStrIndex[0] = outerStrIndex[1] + 1;
      outerStrIndex[1] = (i == maxOuterIndex) ? i + 1 : i;
    }
  }

  if (outerFound > outerIndex) {
    // Extract the inner substring
    String innerData = data.substring(outerStrIndex[0], outerStrIndex[1]);
    int innerFound = 0;
    int innerStrIndex[] = {0, -1};
    int maxInnerIndex = innerData.length() - 1;

    for (int j = 0; j <= maxInnerIndex && innerFound <= innerIndex; j++) {
      if (innerData.charAt(j) == separator || j == maxInnerIndex) {
        innerFound++;
        innerStrIndex[0] = innerStrIndex[1] + 1;
        innerStrIndex[1] = (j == maxInnerIndex) ? j + 1 : j;
      }
    }

    if (innerFound > innerIndex) {
      // Return the inner substring
      return innerData.substring(innerStrIndex[0], innerStrIndex[1]);
    }
  }

  // Return an empty string if not found
  return "";
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
    getValue(data, ',', 0, 1); // Use ',' as both outer and inner separators
  }
}
