#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient client(wifiClient);

class WiFiHandler{
  private:
    const char *ssid = "YourSSID";
    const char *pass = "YourPASS";
    const char *serverIP = "YourMQTT-IP";
    const uint16_t port = 1883;
  public:
    void connectWifi();
    void initializeMQTT(void (*callback)(char*, byte*, unsigned int));
    bool publishData(const char* topic, float _data);
    bool subscribeTopics(const char* topics[]);
    bool checkMQTT();
    bool checkWifi();
};

void WiFiHandler::connectWifi(){
  Serial.print("Connecting to Wifi |  ");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(" X ");
  }
  Serial.println();
  Serial.print("WiFi Connected | IP Address : ");
  Serial.println(WiFi.localIP());
}


void WiFiHandler::initializeMQTT(void (*callbackfunc)(char*, byte*, unsigned int)){
  Serial.println("Connecting to Server | ");
  client.setServer(serverIP, port);
  client.setCallback(callbackfunc);
  Serial.println("Connected to Server");
}

bool WiFiHandler::publishData(const char* topic, float _data){
  Serial.println("Publishing Data");
  char topicData[128];
  dtostrf(_data,2,1,topicData);
  delay(150);
  return client.publish(topic, topicData);
  
}


bool WiFiHandler::subscribeTopics(const char* topics[]){
  Serial.println("Connecting to Topics");
  Serial.print("Topic Size = "); Serial.println(sizeof(topics));
  if (client.connect("ESP32")) {
    for (int i = 0; i < 8; i++) {
      Serial.println(i);
      const char* topicName = topics[i];
      client.subscribe(topicName);
      Serial.println("Subscribed.");
      delay(150);
    }
  }else{
    Serial.print("Error Code :"); Serial.println(client.state());
  }
  return client.connected();
}

bool WiFiHandler::checkMQTT(){
  bool result = client.connected();
  //Serial.print("MQTT => "); Serial.println(result);
  return result;
}

bool WiFiHandler::checkWifi(){
  //Serial.print("Wifi => "); Serial.println(WiFi.status());
  if(WiFi.status() == WL_CONNECTED){
    return true;
  }else{
    return false;
  }
}
