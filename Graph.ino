#include <GyverPortal.h>

GyverPortal portal;

extern float temperatureData[10];
extern float humidityData[10];

void showGraph() {
  GP.BUILD_BEGIN();
  GP.TITLE("ESP-32S График температуры и влажности");

  // Создаем двумерный массив данных для графика
  int16_t data[2][10];
  for (int i = 0; i < 10; i++) {
    data[0][i] = static_cast<int16_t>(temperatureData[i] * 100); // Преобразование в int16_t
    data[1][i] = static_cast<int16_t>(humidityData[i] * 100); // Преобразование в int16_t
  }
  // Вывод данных для отладки
  Serial.println("Temperature Data:");
  for (int i = 0; i < 10; i++) {
    Serial.print(temperatureData[i]);
    Serial.print(", ");
  }
  Serial.println();
  
  Serial.println("Humidity Data:");
  for (int i = 0; i < 10; i++) {
    Serial.print(humidityData[i]);
    Serial.print(", ");
  }
  Serial.println();

  // Массивы подписей для осей
  const char* labels[] = {"Температура", "Влажность"};

  // Отрисовываем график
  GP.PLOT<2, 10>("graph", labels, data, 100);

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