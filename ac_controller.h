#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Mitsubishi.h>

const int acLed = 27;
const int swingLed = 14;
const int pot = 25;
const uint16_t irPin = 5;
int freq = 38;
bool acStatus = false;
IRMitsubishi112 ac(irPin);
String SwingV[] = {"Highest", "High", "Middle", "Low", "Lowest", "Auto"};

void initializeIR() {
  Serial.print("Swing = "); Serial.println(kMitsubishi112SwingVLowest); Serial.println(kMitsubishi112SwingVLow); Serial.println(kMitsubishi112SwingVMiddle);
  Serial.println(kMitsubishi112SwingVHigh); Serial.println(kMitsubishi112SwingVHighest); Serial.println(kMitsubishi112SwingVAuto);
  ac.begin();
  delay(2000);
  ac.setTemp(20);
  ac.setSwingV(kMitsubishi112SwingVLowest); // lowest - low - middle - high - highest - auto
  ac.setMode(kMitsubishi112Cool); // cool - heat - auto - dry
  ac.setFan(kMitsubishi112FanLow); // min - low - med - max
}


void printState() {
  // Display the settings.
  Serial.println("Mitsubishi A/C remote is in the following state:");
  Serial.printf("  %s\n", ac.toString().c_str());
  // Display the encoded IR sequence.
  unsigned char* ir_code = ac.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < kMitsubishiACStateLength; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}


void powerOn(bool sendD = true) {
  Serial.println("Opening A/C");
  ac.setPower(true);
  if (sendD) {
    for (int i = 0; i < 5; i++) {
      ac.send();
    }
  }

  //ac.send();
  digitalWrite(acLed, HIGH);
  acStatus = true;
  delay(2000);

}

void powerOff(bool sendD = true) {
  Serial.println("Closing A/C");
  //sendIR(powerOffData);
  ac.setPower(false );
  if (sendD) {
    for (int i = 0; i < 5; i++) {
      ac.send();
    }
  }
  digitalWrite(acLed, LOW);
  acStatus = false;
  delay(2000);
}

void setSwingVAC(int buttonPin) {
  int potV;
  int swingCode;
  int analogPower;
  while (digitalRead(buttonPin) == LOW) {
    potV = analogRead(pot);
    swingCode = map(potV, 0, 4096, 1, 6);
    analogPower = 255 / swingCode; // 42 * 6 = 252 (apprx. 256)
    analogWrite(swingLed, analogPower),
                Serial.print("Swing Code = "); Serial.println(swingCode);
    Serial.print("Swing Mode = "); Serial.println(SwingV[ swingCode - 1 ]);
    delay(750);
    if (digitalRead(buttonPin)) {
      analogWrite(swingLed, LOW);
      Serial.println("Release Button");
      delay(2000);
      break;
    }
  }
  if (swingCode == 6) {
    swingCode = 7;
  }
  ac.setSwingV(swingCode);
  ac.send();
}

bool handleAC(float temperature, long timeDifferenceAC, bool firstRun) {
  if ( (temperature > 29 || temperature == 29) && acStatus == false && (timeDifferenceAC > 600000 || firstRun) ) { // 10 mins
    firstRun = false;
    powerOn();
    return true;
  }
  else if (
    ( (temperature < 27 && (timeDifferenceAC > 600000 || firstRun)) || (temperature < 24) ) && acStatus == true) {
    firstRun = false;
    powerOff();
    return true;
  }
  return false;
}
