#include <EEPROM.h>

#define EEPROM_SIZE 4096

void setup_eeprom() {
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("Не удалось инициализировать EEPROM");
    while (1); // Вместо микроконтроллера бездушный камень, реализовать перезагрузку
  }
   readEEPROM();
   readChartData(); // чтение данных графика из EEPROM
}

void saveEEPROM() {
  for (size_t i = 0; i < sizeof(ssidwifi); ++i) {
    EEPROM.write(i, ssidwifi[i]);
  }
  for (size_t i = 0; i < sizeof(passwifi); ++i) {
    EEPROM.write(sizeof(ssidwifi) + i, passwifi[i]);
  }
  for (size_t i = 0; i < sizeof(BOTtoken); ++i) {
    EEPROM.write(sizeof(ssidwifi) + sizeof(passwifi) + i, BOTtoken[i]);
  }
  for (size_t i = 0; i < sizeof(CHAT_ID); ++i) {
    EEPROM.write(sizeof(ssidwifi) + sizeof(passwifi) + sizeof(BOTtoken) + i, CHAT_ID[i]);
  }
  EEPROM.commit();
}

void readEEPROM() {
  EEPROM.get(0, ssidwifi);
  EEPROM.get(sizeof(ssidwifi), passwifi);
  EEPROM.get(sizeof(ssidwifi) + sizeof(passwifi), BOTtoken);
  EEPROM.get(sizeof(ssidwifi) + sizeof(passwifi) + sizeof(BOTtoken), CHAT_ID);
}

// работа с графиком
void updateChartData() {
  Temperature = dht.readTemperature();
  Humidity = dht.readHumidity();

  for (int i = 8; i >= 0; i--) {
    temperatureData[i + 1] = temperatureData[i];
    humidityData[i + 1] = humidityData[i];
  }
  temperatureData[0] = Temperature;
  humidityData[0] = Humidity;

  saveChartData();
}

// работа с графиком
void saveChartData() {
  int addr = sizeof(ssidwifi) + sizeof(passwifi) + sizeof(BOTtoken) + sizeof(CHAT_ID);
  EEPROM.put(addr, temperatureData);
  EEPROM.put(addr + sizeof(temperatureData), humidityData);
  EEPROM.commit();
}

// работа с графиком
void readChartData() {
  int addr = sizeof(ssidwifi) + sizeof(passwifi) + sizeof(BOTtoken) + sizeof(CHAT_ID);
  EEPROM.get(addr, temperatureData);
  EEPROM.get(addr + sizeof(temperatureData), humidityData);
}