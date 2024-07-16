#include <WiFi.h>
#include <WiFiClientSecure.h>

WiFiClientSecure client;

void setup_wifi() {
#ifdef ESP32
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
#endif
  WiFi.mode(WIFI_STA);
  WiFi_Connect();
}

void startAP() {
  if (!apStarted) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(apSSID, apPassword);
    Serial.println("Точка доступа запущена");
    Serial.print("IP адрес ESP32: ");
    Serial.println(WiFi.softAPIP());
    apStarted = true;
  }
}
void stopAP() {
  if (apStarted) {
    WiFi.softAPdisconnect(true);  // Отключение точки доступа
    Serial.println("Точка доступа отключена");
    apStarted = false;
  }
}


void WiFi_Connect() {
  Serial.println();
  Serial.print("Подключение к ");
  Serial.println(ssidwifi);
  WiFi.begin(ssidwifi, passwifi);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    Serial.print(".");
    delay(1000);
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi подключено!");
    Serial.print("Ваш IP адрес: ");
    Serial.println(WiFi.localIP());
    if (bot == nullptr) {
      bot = new UniversalTelegramBot(BOTtoken, client);
    }
    WiFiPing = true;
  }
}

void work_wifi() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!wifiConnected) {
      wifiConnected = true;
      Serial.println("WiFi подключение успешно!");
      Serial.print("Ваш IP адрес: ");
      Serial.println(WiFi.localIP());
      if (bot == nullptr) {
        bot = new UniversalTelegramBot(BOTtoken, client);
      }
    }
  } else {
    if (wifiConnected) {
      wifiConnected = false;
      Serial.println("Нет подключения к WiFi!");
      WiFi_Connect();
    }
  }
}
