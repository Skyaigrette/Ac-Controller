#include <DHT.h>
#define DHTPIN 13
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);
void initializeDHT(){
  dht.begin();
  delay(2000);
}

float calculateTemperature(){
  return dht.readTemperature();
}

float calculateHumidity(){
  return dht.readHumidity();
}
