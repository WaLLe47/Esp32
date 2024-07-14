#include <DHT.h>

#define DHTPIN 13
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup_dht() {
  dht.begin();
}

void  work_dht() {
  Temperature = dht.readTemperature();
  Humidity = dht.readHumidity();
}