#include <DallasTemperature.h>
#include <OneWire.h>


 
// Провод данных подключен к контакту 2 на Arduino
#define ONE_WIRE_BUS 2
 
// Настройка oneWire для связи с любыми устройствами OneWire 
// (не только Maxim/Dallas температурные IC)
OneWire oneWire(ONE_WIRE_BUS);
 
DallasTemperature sensors(&oneWire);
 
void setup(void)
{
  // Старт серийного порта
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC демо");

  // Запуск библиотеки
  sensors.begin();
} 
 
void loop(void)
{
  // запрашиваем sensor.requestTemperatures() для получения глобальной температуры
  // запрос всех устройств на шине
  Serial.print(" Запрашиваем температуру...");
  sensors.requestTemperatures(); // Отправляем команды для получения температуры
  Serial.println("DONE");

  Serial.print("Температура: ");
  Serial.print(sensors.getTempCByIndex(0)); // Почему "byIndex"? 
    // У вас может быть несколько IC на одной шине. 
    // 0 относится к первой IC
    delay(10000);
}
