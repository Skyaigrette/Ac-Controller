#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient client(wifiClient);

class WiFiHandler{
  private:
    const char *ssid = "AricasF";
    const char *pass = "furkan2005!";
    const char *serverIP = "192.168.1.48";
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
  Serial.print("Connecting to Topics");
  if (client.connect("ESP32")) {
    for (int i = 0; i < 2; i++) {
      const char* topicName = topics[i];
      client.subscribe(topicName);
      Serial.println("Subscribed.");
    }
  }else{
    Serial.print("Error Code :"); Serial.println(client.state());
  }
  return client.connected();
}

bool WiFiHandler::checkMQTT(){
  bool result = client.connected();
  Serial.print("MQTT => "); Serial.println(result);
  return result;
}

bool WiFiHandler::checkWifi(){
  Serial.print("Wifi => "); Serial.println(WiFi.status());
  if(WiFi.status() == WL_CONNECTED){
    return true;
  }else{
    return false;
  }
}





/*#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient client(wifiClient);

#define ssid  "AricasF"
#define pass  "furkan2005!"
#define mqtt_server  "192.168.1.48"
#define tempTopic  "sensors/dht11/temperature"
#define humidityTopic "sensors/dht11/humidity"
#define username  "cl1"
#define mqtt_port  1883
bool connectionState = false;

 
String client_id = "esp-client-";
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

void connectWiFi(){
  WiFi.begin(ssid, pass);
  Serial.print("Connecting on WiFi =");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(" X ");
  }
  Serial.println(); Serial.println("Connected on WiFi");
}

void connectMQTT(){
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  Serial.print("Connecting MQTT || ");
  while(!client.connected()){
    Serial.print("X"); 
    if (client.connect(client_id.c_str(), username, pass)) {
        Serial.println("Connected");
    } else {
        Serial.print("Failed With State = ");
        Serial.println(client.state());
    }    
    delay(500);
  } 
}

void subscribeTopics(){
  client.subscribe(tempTopic);
  client.subscribe(humidityTopic); 
}
void publishMessage(float temperature, float humidity){
  char tempString[8];
  char humidString[8];
  dtostrf(temperature,1,2,tempString);
  dtostrf(humidity,1,2,humidString);
  client.publish(tempTopic, tempString);
  client.publish(humidityTopic, humidString);
}

void initializeWifiMQTT(){
  connectWiFi();
  connectMQTT();
  subscribeTopics();
}

void checkConnections(){
  if(WiFi.status() != WL_CONNECTED){
    connectionState = false; 
    Serial.print("No WiFi Connection");
    connectWiFi();

  }
  if(!client.connected()){
    connectionState = false;
    Serial.print("No MQTT Connection");
    connectMQTT();
    subscribeTopics();
    
  }
  Serial.println("No problem with connections.");
  connectionState = true;
}
  */
