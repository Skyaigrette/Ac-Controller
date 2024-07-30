#include "ac_controller.h"
#include "dht_controller.h"
#include "tester.h"
#include "wifi_controller.h"
#include "second_thread.h"
#include "utils.h"
unsigned long oldBoardTimeCalculation = 0;
unsigned long oldBoardTimeAC = 0;
unsigned long newBoardTime;
float temperature;
float humidity;
unsigned long timeDifferenceCalculation;
unsigned long timeDifferenceAC;
bool firstRun = true;
float lastTempData;
float lastHumidData;

void setup() {
  Serial.begin(115200, SERIAL_8N1);
  pinMode(irPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(acLed, OUTPUT);
  pinMode(swingLed, OUTPUT);
  pinMode(pot, INPUT);
  initializeIR();
  initializeDHT();
  //test();
  //powerOff();
  delay(2000);
  //  MQTT DISABLED
  xTaskCreatePinnedToCore(
    Task1code, /* Function to implement the task */
    "Task1", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &Task1,  /* Task handle. */
    0);

}

void loop() {
  //    checkConnections();
  newBoardTime = millis();
  timeDifferenceCalculation = newBoardTime - oldBoardTimeCalculation;
  timeDifferenceAC = newBoardTime - oldBoardTimeAC;
  if (timeDifferenceCalculation > 2000) {
    temperature = calculateTemperature();
    humidity = calculateHumidity();

    if (lastTempData != temperature) {
      wifi.publishData(temperatureTopic, temperature);
    }
    if (lastHumidData != humidity) {
      wifi.publishData(humidityTopic, humidity);
    }

    Serial.print(temperature); Serial.print(","); Serial.println(humidity);
    lastTempData = temperature;
    lastHumidData = humidity;
    oldBoardTimeCalculation = newBoardTime;
    Serial.print("AC Status = "); Serial.print(acStatus); Serial.print(" || Time Difference = "); differenceToTime(timeDifferenceAC);

  }

  if (autoMode && handleAC(temperature, timeDifferenceAC, firstRun)) { // if autoMode is false, it won't run handleAC at all.
    oldBoardTimeAC = newBoardTime;
  }
  if (digitalRead(buttonPin) == HIGH) {
    digitalWrite(swingLed, HIGH); delay(200); digitalWrite(swingLed, LOW);
    Serial.println("Set SWING Mode");
    delay(2000);
    setSwingVAC(buttonPin);
  }
  if (sender) {
    for (int i = 0; i < 5; i++) {
      ac.send();
    }
    Serial.println("Ac Sent.");
    sender = false;
  }
  delay(50);
}
