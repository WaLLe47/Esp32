// Глобальные переменные
float Temperature;
float Humidity;

char ssidwifi[30];
char passwifi[30];
char BOTtoken[50];
char CHAT_ID[20];

const char* apSSID = "ESP32";
const char* apPassword = "password";

bool wifiConnected = false;
bool apStarted = false;
bool WiFiPing = false;

uint32_t TimerAll = millis();
uint32_t chartUpdateTimer = millis();
uint32_t hourlyUpdateTimer = millis();

// интервал обновления графика 60 секунд
#define CHART_UPDATE_INTERVAL 60000 

// интервал обновления часового графика 60 минут
#define HOURLY_UPDATE_INTERVAL 3600000

float temperatureData[60]; // массив для хранения последних 60 значений температуры (данные за час)
float humidityData[60];    // массив для хранения последних 60 значений влажности (данные за час)
uint32_t timeData[60];     // массив для хранения временных меток (данные за час)

float hourlyTemperatureData[24]; // массив для хранения последних 24 значений температуры (данные за 24 часа)
float hourlyHumidityData[24];    // массив для хранения последних 24 значений влажности (данные за 24 часа)
uint32_t hourlyTimeData[24];     // массив для хранения временных меток (данные за 24 часа)

float tempSum = 0; // переменные для суммирования данных за час
float humSum = 0;
int minuteCount = 0; // счетчик минут для расчета среднего значения

void setup() {
  Serial.begin(115200);
  delay(1000);
  setup_dht();
  setup_btn();
  setup_eeprom();
  Serial.println("EEPROM инициализирована");
  setup_wifi();
  setup_portal();
  Serial.println("HTTP сервер запущен");
  WiFi_Connect();
}

void loop() {
  work_portal();
  if (WiFiPing && (millis() - TimerAll) > 5000) {
    work_wifi();
    work_telegram();
    TimerAll = millis();
  }
  if ((millis() - chartUpdateTimer) > CHART_UPDATE_INTERVAL) {
    updateChartData();
    chartUpdateTimer = millis();
  }
  if ((millis() - hourlyUpdateTimer) > HOURLY_UPDATE_INTERVAL) {
    updateHourlyData();
    hourlyUpdateTimer = millis();
  }
  switch (work_btn()) {
    case 2:
      startAP();
      break;
    case 1:
      work_dht();
      work_telegram_send();
      break;
  }
}