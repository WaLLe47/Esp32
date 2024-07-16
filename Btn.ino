
// Параметр кнопки
#define BUTTON_PIN 23
unsigned long pressStartTime = 0;
unsigned long pressDuration = 0;

void setup_btn() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

int work_btn() {  // Обработка состояния кнопки если меньше 1500мс возвращаем 1, больше 2, если не нажимали 0

  if (digitalRead(BUTTON_PIN) == LOW) {
    pressStartTime = millis();

    // Ждем отпускания кнопки
    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(100);
    }

    pressDuration = millis() - pressStartTime;
    
    Serial.print("Кнопка была зажата в течение ");
    Serial.print(pressDuration);
    Serial.println(" миллисекунд");
    
    if (pressDuration < 500) return 1;
    return 2;
  } else {
    return 0;
  }
}
