// ДЕМО движение до объекта спереди. (стенка)

#include <Arduino.h>
#include <MoshLib.hpp> // Подключаем библиотеку для мобильного робота

void setup() {
  // Конфигурирование робота

  // Задаём передний датчик расстояния
  setDistF(us); // Ультразвуковой
  // setDistF(ir0); // Инфракрасный-0
  // setDistF(ir1); // Инфракрасный-1

  delay(1000); // Задержка перед стартом робота

  // Исполнение алгоритма

  wallFront(30); // Движение прямо вперёд до стены на расстоянии 30
  // wallFront(ir1, 30, 20); // Вариант без конфигурации - передаём какой именно датчик опрашивать, расстояние - 30, скорость - 20 тиков

  goDist(-500); // Когда доехали, объезжаем назад
}

void loop() {
  // Ничего не нужно делать после
}
