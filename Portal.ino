IPAddress ip1(192, 168, 4, 2);

void setup_portal() {
  portal.attach(action);
  portal.attachBuild(showGraph);
  portal.start("esp32");  //запуск с поддержкой mDNS,
  timeClient.begin();
  timeClient.setTimeOffset(28800);
}
void work_portal() {
  portal.tick();
  timeClient.update();
}

void build() {
  GP.BUILD_BEGIN();
  GP.FORM_BEGIN("/configuration");
  GP.PAGE_TITLE("ESP-32S");
  GP.TITLE("ESP-32S Конфигурация");
  GP.TEXT("ssid", "Wi-Fi SSID");
  GP.TEXT("password", "Wi-Fi Пароль");
  GP.TEXT("token", "Токен Telegram Бота");
  GP.TEXT("chat_id", "Chat Id");
  GP.BREAK();
  GP.SUBMIT("Отправить даные для подключения");
  GP.FORM_END();
  GP.BUILD_END();
}

void noWifi() {
  GP.BUILD_BEGIN();
  GP.FORM_BEGIN("/noWifi");
  GP.TITLE("ESP-32S Нет подключения к сети");
  GP.SPAN("Не удалось подключиться. Пожалуйста, попробуйте снова");
  GP.SUBMIT("Вернуться");
  GP.FORM_END();

  GP.BUILD_END();
}

void yesWifi() {
  GP.BUILD_BEGIN();
  GP.FORM_BEGIN("/yesWifi");
  GP.TITLE("ESP-32S Подключилось к сети WiFi");
  GP.SPAN("Подключение к сети есть,для продолжения подключите устройство к WiFi, к которому подключен esp32 и перейдите по адресу:");
  GP.SPAN("http://esp32 или http://" + WiFi.localIP().toString());
  GP.SUBMIT("Отключить точку доступа");
  GP.FORM_END();
  GP.BUILD_END();
}

void action() {
  IPAddress clientIP = portal.clientIP();
  Serial.println(portal.clientIP());
  if (clientIP == ip1) {
    portal.attachBuild(build);
  } else {
    portal.attachBuild(showGraph);
  }

  if (portal.form("/configuration")) {
    strncpy(ssidwifi, portal.arg("ssid").c_str(), sizeof(ssidwifi) - 1);
    strncpy(passwifi, portal.arg("password").c_str(), sizeof(passwifi) - 1);
    strncpy(BOTtoken, portal.arg("token").c_str(), sizeof(BOTtoken) - 1);
    strncpy(CHAT_ID, portal.arg("chat_id").c_str(), sizeof(CHAT_ID) - 1);

    WiFi_Connect();
    if (WiFi.status() == WL_CONNECTED) {
      saveEEPROM();
      WiFiPing = true;
      portal.attachBuild(yesWifi);
    } else {
      WiFiPing= false; 
      portal.attachBuild(noWifi);
    }
  } else if (portal.form("/noWifi")) {
    portal.attachBuild(build);
  } else if (portal.form("/chart")) {
    portal.attachBuild(showGraph);
  } else if (portal.form("/yesWifi")) {
    stopAP();
  }
}