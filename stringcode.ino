//"(54,1000),(60,1500),(80,2000),(54,1000),(60,1500),(80,2000),(54,1000),(60,1500),(80,2000)"

#include <Arduino.h>

String getValue(String data, char outerSeparator, char innerSeparator, int outerIndex, int innerIndex) {
  int outerFound = 0;
  int outerStrIndex[] = {0, -1};
  int maxOuterIndex = data.length() - 1;

  for (int i = 0; i <= maxOuterIndex && outerFound <= outerIndex; i++) {
    if (data.charAt(i) == outerSeparator || i == maxOuterIndex) {
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
      if (innerData.charAt(j) == innerSeparator || j == maxInnerIndex) {
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
  Serial.begin(115200);
}

void loop() {
  String data = "(54,1000),(60,1500),(80,2000),(54,1000),(60,1500),(80,2000),(54,1000),(60,1500),(80,2000)";
  char outerSeparator = ',';
  char innerSeparator = ',';

  int substringIndex = 0;
  int firstValueIndex = 0;
  int secondValueIndex = 1;

  while (substringIndex < 10) {
    // Extract the first and second values from each substring
    String firstValue = getValue(data, outerSeparator, innerSeparator, substringIndex, firstValueIndex);
    String secondValue = getValue(data, outerSeparator, innerSeparator, substringIndex, secondValueIndex);

    // Check if values are not empty
    if (firstValue != "" && secondValue != "") {
      Serial.print("Variable 1: ");
      Serial.println(firstValue);
      Serial.print("Variable 2: ");
      Serial.println(secondValue);
    } else {
      // Break the loop if values are empty
      break;
    }

    // Move to the next substring
    substringIndex++;
  }

  // Wait for a while before restarting the loop
  delay(5000);
}
