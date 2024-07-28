  
#include "ac_controller.h"
#include "dht_controller.h"
#include "tester.h"
#include "wifi_controller.h"
#include "second_thread.h"
unsigned long oldBoardTimeCalculation = 0;
unsigned long oldBoardTimeAC = 0;
unsigned long newBoardTime;
float temperature;
float humidity;
unsigned long timeDifferenceCalculation;
unsigned long timeDifferenceAC;
bool firstRun = true;

void setup() {
  Serial.begin(115200, SERIAL_8N1);
  pinMode(irPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(acLed, OUTPUT);
  pinMode(swingLed, OUTPUT);
  pinMode(pot, INPUT);
  initializeIR();  
  initializeDHT();
  test();
  powerOff();

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
    if(timeDifferenceCalculation > 2000){
      temperature = calculateTemperature();
      humidity = calculateHumidity();
      wifi.publishData(temperatureTopic, temperature);
      //wifi.publishData(humidityTopic, humidity);
      Serial.print(temperature); Serial.print(","); Serial.println(humidity);
      oldBoardTimeCalculation = newBoardTime;
         Serial.print("AC Status = "); Serial.print(acStatus); Serial.print(" || Time Difference = "); Serial.println(timeDifferenceAC);

  }

   if( (temperature > 29 || temperature == 29) && acStatus == false && (timeDifferenceAC > 600000 || firstRun) ){ // 10 mins
    firstRun = false;
    oldBoardTimeAC = newBoardTime;
    powerOn();
   }
   else if(
    ( (temperature < 27 && (timeDifferenceAC > 600000 || firstRun)) || (temperature < 24) ) && acStatus == true){
      firstRun = false;
      oldBoardTimeAC = newBoardTime;
      powerOff();
    }
    if(digitalRead(buttonPin) == HIGH){
      Serial.println("Set SWING Mode");
      delay(2000);
      setSwingVAC(buttonPin);
    }
    delay(50);  
}
