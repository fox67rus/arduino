// Подключаем библиотеку
#include "Adafruit_CCS811.h"
// По умолчанию адрес датчика в библиотеке 0x5A
Adafruit_CCS811 ccs;
void setup()
{
  Serial.begin(9600);
  Serial.println("CCS811 test");
  if (!ccs.begin())
  {
    Serial.println("Failed to start sensor! Please check your wiring."); // Если при инициализации на старте датчик не найден,
    while (1);                                                           // в мониторе порта вы получите уведомление об этом.                                                                      
  }
  //calibrate temperature sensor
 /* while (!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);*/
}
void loop()
{
  if (ccs.available())
  {
    //float temp = ccs.calculateTemperature();
    if (!ccs.readData())
    {
      Serial.print("eCO2: ");                                              // Значение уровня eCO2
      Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");                                        // Значение уровня TVOC
      Serial.println(ccs.getTVOC());
      //Serial.print("ppb   Temp:");                                        // Температура
     // Serial.println(temp);
    }
    else
    {
      Serial.println("ERROR!");
      while (1);
    }
  }
  delay(500);
}
