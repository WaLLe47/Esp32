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

// интервал обновления графика 60 секунд
#define CHART_UPDATE_INTERVAL 60000 

float temperatureData[10]; // массив для хранения последних 10 значений температуры
float humidityData[10];    // массив для хранения последних 10 значений влажности

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
  if (WiFiPing && (millis() - TimerAll)>5000) {
    work_wifi();
    work_telegram();
    TimerAll = millis();
  }
  if ((millis() - chartUpdateTimer) > CHART_UPDATE_INTERVAL) {
    updateChartData();
    chartUpdateTimer = millis();
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
