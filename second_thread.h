#include <ctype.h>
#include "callback_conditions.h"
TaskHandle_t Task1;
WiFiHandler wifi;
const char* temperatureTopic = "sensors/dht11/temperature";
const char* humidityTopic = "sensors/dht11/humidity";
const char* autoModeTopic = "phone/auto-mod";
const char* acPowerTopic = "phone/ac/power";
const char* acTempTopic = "phone/ac/temperature";
const char* acModeTopic = "phone/ac/mode";
const char* acSwingVTopic = "phone/ac/swing-v";
const char* acFanTopic = "phone/ac/fan";
bool sender = false;
const char *topics[] = {temperatureTopic, humidityTopic, autoModeTopic, acPowerTopic, acTempTopic, acModeTopic, acSwingVTopic, acFanTopic};
long lastReconnectAttempt = 0;



void callback(char *topic, byte *payload, unsigned int length) {
  String message;
  Serial.println(topic);
  for (int i = 0; i < length; i++) {
    message += (char) payload[i];
  }
  Serial.println(message);

    if(  // if topic matches, it runs handle command. if topic does not match it skips to next topic.
    (strcmp(topic, autoModeTopic) == 0 && handleAutoMode(message)) ||
    (strcmp(topic, acTempTopic) == 0 && handleTemperature(message)) ||
    (strcmp(topic, acModeTopic) == 0 && handleMode(message)) ||
    (strcmp(topic, acSwingVTopic) == 0 && handleSwingV(message)) ||
    (strcmp(topic, acPowerTopic) == 0 && handlePower(message)) ||
    (strcmp(topic, acFanTopic) == 0 && handleFan(message))
    ){
      sender = true; //  IDK why but IR Led doesn't send any valid data to AC. LED works but AC doesn't get any commands. Via this variable, we'll be able to send command on first core. 
    } else{
      sender = false;
    } 


  }


void Task1code( void * parameter) {
  ac.begin();
  delay(2000);
  wifi.connectWifi();
  wifi.initializeMQTT(callback);
  wifi.subscribeTopics(topics);
  for (;;) {
    if (!wifi.checkMQTT()) {
      Serial.println("MQTT Connection Problem. Reconnecting.");
      wifi.subscribeTopics(topics);
    }
    if (!wifi.checkWifi()) {
      Serial.println("Wifi Connection Problem Reconnecting.");
      wifi.connectWifi();
    }
    client.loop();
    delay(500);
  }
}
