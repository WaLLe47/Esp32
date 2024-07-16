#include <UniversalTelegramBot.h>

UniversalTelegramBot* bot = nullptr;

void work_telegram() {
  if (bot != nullptr) {
    int numNewMessages = bot->getUpdates(bot->last_message_received + 1);
    if (numNewMessages > 0) {
      handleNewMessages(numNewMessages);
    }
  }
}

void work_telegram_send() {
  // Проверка на ошибки чтения датчика
  if (isnan(Temperature) || isnan(Humidity)) {
    Serial.println("Ошибка чтения с датчика DHT22");
    String message = "Ошибка чтения с датчика DHT22";
    bot->sendMessage(CHAT_ID, message, "");
  } else {
    Serial.print("Температура: ");
    Serial.print(Temperature);
    Serial.println(" °C");
    Serial.print("Влажность: ");
    Serial.print(Humidity);
    Serial.println(" %");
    String message = "Вот отчет по температуре и влажности:\n";
    message += "Температура: " + String(Temperature) + " °C\n";
    message += "Влажность: " + String(Humidity) + " %\n";
    bot->sendMessage(CHAT_ID, message, "");
    Serial.print("Кнопка была нажата в течение ");
    Serial.print(pressDuration);
    Serial.println(" миллисекунд");
  }
}




void handleNewMessages(int numNewMessages) {
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot->messages[i].chat_id);

    if (chat_id != CHAT_ID) {
      bot->sendMessage(chat_id, "У вас нет доступа", "");
      continue;
    }

    String text = bot->messages[i].text;

    if (text == "/start") {
      bot->sendMessage(chat_id, "Привет", "");
    } else if (text == "/status") {
      // Считывание данных с датчика DHT22
      Temperature = dht.readTemperature();
      Humidity = dht.readHumidity();
      // Проверка на ошибки чтения датчика
      if (isnan(Temperature) || isnan(Humidity)) {
        Serial.println("Ошибка чтения с датчика DHT22");
        String message = "Ошибка чтения с датчика DHT22";
        bot->sendMessage(chat_id, message, "");
      } else {
        Serial.print("Температура: ");
        Serial.print(Temperature);
        Serial.println(" °C");
        Serial.print("Влажность: ");
        Serial.print(Humidity);
        Serial.println(" %");
        String message = "Вот отчет по температуре и влажности:\n";
        message += "Температура: " + String(Temperature) + " °C\n";
        message += "Влажность: " + String(Humidity) + " %\n";
        bot->sendMessage(chat_id, message, "");
      }
    }
  }
}
