TaskHandle_t Task1;
WiFiHandler wifi;
const char* temperatureTopic = "sensors/dht11/temperature";
const char* humidityTopic = "sensors/dht11/humidity";
const char *topics[] = {temperatureTopic, humidityTopic};
long lastReconnectAttempt = 0;

void callback(char *topic, byte *payload, unsigned int length){
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void Task1code( void * parameter) {
  wifi.connectWifi();
  wifi.initializeMQTT(callback);
  wifi.subscribeTopics(topics);
  for(;;) {
    Serial.println("Second Thread");
    if(!wifi.checkMQTT()){
      Serial.println("MQTT Connection Problem. Reconnecting.");
      wifi.subscribeTopics(topics);
    }
    if(!wifi.checkWifi()){
      Serial.println("Wifi Connection Problem Reconnecting.");
      wifi.connectWifi();
    }
    client.loop();
    delay(500);
  }
}
