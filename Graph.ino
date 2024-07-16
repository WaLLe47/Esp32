#include <GyverPortal.h>

GyverPortal portal;

extern float temperatureData[60];
extern float humidityData[60];
extern uint32_t timeData[60];
extern float hourlyTemperatureData[24];
extern float hourlyHumidityData[24];
extern uint32_t hourlyTimeData[24];

void createLabels(const uint32_t* timeData, int count, char labels[][6]) {
  for (int i = 0; i < count; ++i) {
    int minutes = (timeData[i]) % 60;
    int hours = (timeData[i] / 3600) % 24;
    sprintf(labels[i], "%02d:%02d", hours, minutes);
    Serial.print("Label ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(labels[i]);
  }
  Serial.println("-------------------------------");
}

void showGraph() {
  GP.BUILD_BEGIN();
  GP.TITLE("ESP-32S График температуры и влажности");

  char minuteLabels[60][6];
  createLabels(timeData, 60, minuteLabels);
  const char* minuteLabelPtrs[60];
  for (int i = 0; i < 59; ++i) {
    minuteLabelPtrs[i] = minuteLabels[i];
  }

  char hourlyLabels[24][6];
  createLabels(hourlyTimeData, 23, hourlyLabels);
  const char* hourlyLabelPtrs[24];
  for (int i = 0; i < 24; ++i) {
    hourlyLabelPtrs[i] = hourlyLabels[i];
  }

  int16_t minuteData[2][60];
  int16_t hourlyData[2][24];

  for (int i = 0; i < 60; ++i) {
    minuteData[0][i] = static_cast<int16_t>(temperatureData[i] * 100);
    minuteData[1][i] = static_cast<int16_t>(humidityData[i] * 100);
  }

  for (int i = 0; i < 24; ++i) {
    hourlyData[0][i] = static_cast<int16_t>(hourlyTemperatureData[i] * 100);
    hourlyData[1][i] = static_cast<int16_t>(hourlyHumidityData[i] * 100);
  }

  GP.PLOT_STOCK<2, 60>("График за последний час", minuteLabelPtrs, timeData, minuteData, 100);
  GP.PLOT_STOCK<2, 24>("График за последние 24 часа", hourlyLabelPtrs, hourlyTimeData, hourlyData, 100);

  Temperature = dht.readTemperature();
  Humidity = dht.readHumidity();
  GP.LABEL("Текущая температура: ");
  GP.LABEL(String(Temperature) + " °C");
  GP.BREAK();
  GP.LABEL("Текущая влажность: ");
  GP.LABEL(String(Humidity) + " %");
  GP.BREAK();

  GP.FORM_BEGIN("/chart");
  GP.SUBMIT("Обновить график");
  GP.FORM_END();

  GP.BUILD_END();
}