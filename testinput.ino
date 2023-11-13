#include <Arduino.h>

// Define relay pins
int motorPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};

//pump runtime
const float FLOW_RATE = 1.5; // in ml/minute

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

void mixCocktail(int pumpNumber, int runTime) {
  Serial.print("Mixing pump: ");
  Serial.println(pumpNumber);

  // Activate the specified pump motor
  activateMotor(motorPins[pumpNumber - 1], runTime);

  Serial.print("Pump ");
  Serial.print(pumpNumber);
  Serial.println(" Completed");
}

void inputCmd(String commandStr) {
  commandStr.trim();

  // Split multiple commands separated by commas
  int commandCount = 0;
  String commands[5]; // Assuming a maximum of 5 commands
  while (commandStr.length() > 0) {
    commands[commandCount] = commandStr.substring(1, commandStr.indexOf(')'));
    commandStr = commandStr.substring(commandStr.indexOf(')') + 2);
    commandCount++;
  }

  // Process each command
  for (int i = 0; i < commandCount; i++) {
    int pumpNumber = getValue(commands[i], ',', 0).toInt();
    int runTime = getValue(commands[i], ',', 1).toInt();
    mixCocktail(pumpNumber, runTime);
  }
}

void setup() {
  // Setup relay pins as OUTPUT
  for (int i = 0; i < sizeof(motorPins) / sizeof(motorPins[0]); i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    inputCmd(data);
  }
}
