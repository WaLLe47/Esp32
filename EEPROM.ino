#include <EEPROM.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600);

#define EEPROM_SIZE 4096

void setup_eeprom() {
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("Не удалось инициализировать EEPROM");
    while (1)
      ;  // Вместо микроконтроллера бездушный камень, реализовать перезагрузку
  }
  /*for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0);  // Записываем 0 во все адреса EEPROM
  }
  EEPROM.commit();  // Применяем изменения в EEPROM*/
  readEEPROM();
  readChartData();  // чтение данных графика из EEPROM
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

// работа с графиком за час
void updateChartData() {
  Temperature = dht.readTemperature();
  Humidity = dht.readHumidity();
  uint32_t currentTime = timeClient.getEpochTime();  // Получаем текущее время только один раз

  // Сдвиг данных
  for (int i = 58; i >= 0; i--) {
    temperatureData[i + 1] = temperatureData[i];
    humidityData[i + 1] = humidityData[i];
    timeData[i + 1] = timeData[i];
  }

  // Запись новых данных
  temperatureData[0] = Temperature;
  humidityData[0] = Humidity;
  timeData[0] = currentTime;

  Serial.println(timeData[0]);

  tempSum += Temperature;
  humSum += Humidity;
  minuteCount++;

  saveChartData();
}

void updateHourlyData() {
  float tempAverage = tempSum / minuteCount;
  float humAverage = humSum / minuteCount;
  uint32_t currentTime = timeClient.getEpochTime();  // Получаем текущее время только один раз

  // Сдвиг данных
  for (int i = 22; i >= 0; i--) {
    hourlyTemperatureData[i + 1] = hourlyTemperatureData[i];
    hourlyHumidityData[i + 1] = hourlyHumidityData[i];
    hourlyTimeData[i + 1] = hourlyTimeData[i];
  }

  // Запись новых данных
  hourlyTemperatureData[0] = tempAverage;
  hourlyHumidityData[0] = humAverage;
  hourlyTimeData[0] = currentTime;

  tempSum = 0;
  humSum = 0;
  minuteCount = 0;

  saveHourlyData();
}


// работа с графиком
void saveChartData() {
  int addr = sizeof(ssidwifi) + sizeof(passwifi) + sizeof(BOTtoken) + sizeof(CHAT_ID);
  EEPROM.put(addr, temperatureData);
  EEPROM.put(addr + sizeof(temperatureData), humidityData);
  EEPROM.put(addr + sizeof(temperatureData) + sizeof(humidityData), timeData);
  EEPROM.commit();
}

void readChartData() {
  int addr = sizeof(ssidwifi) + sizeof(passwifi) + sizeof(BOTtoken) + sizeof(CHAT_ID);
  EEPROM.get(addr, temperatureData);
  EEPROM.get(addr + sizeof(temperatureData), humidityData);
  EEPROM.get(addr + sizeof(temperatureData) + sizeof(humidityData), timeData);
}

void saveHourlyData() {
  int addr = sizeof(ssidwifi) + sizeof(passwifi) + sizeof(BOTtoken) + sizeof(CHAT_ID) + sizeof(temperatureData) + sizeof(humidityData) + sizeof(timeData);
  EEPROM.put(addr, hourlyTemperatureData);
  EEPROM.put(addr + sizeof(hourlyTemperatureData), hourlyHumidityData);
  EEPROM.put(addr + sizeof(hourlyTemperatureData) + sizeof(hourlyHumidityData), hourlyTimeData);
  EEPROM.commit();
}

void readHourlyData() {
  int addr = sizeof(ssidwifi) + sizeof(passwifi) + sizeof(BOTtoken) + sizeof(CHAT_ID) + sizeof(temperatureData) + sizeof(humidityData) + sizeof(timeData);
  EEPROM.get(addr, hourlyTemperatureData);
  EEPROM.get(addr + sizeof(hourlyTemperatureData), hourlyHumidityData);
  EEPROM.get(addr + sizeof(hourlyTemperatureData) + sizeof(hourlyHumidityData), hourlyTimeData);
}