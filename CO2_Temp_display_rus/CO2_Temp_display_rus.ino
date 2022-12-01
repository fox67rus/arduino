// Подключаем библиотеку
#include "Adafruit_CCS811.h"
#include <DHT.h>
#include <UTFT.h>
#include <stdlib.h> // included for floatToString function

// По умолчанию адрес датчика в библиотеке 0x5A
Adafruit_CCS811 ccs;

// пин для подключения датчика DHT
#define DHTPIN 3     // what pin we're connected to
// тип датчика DHT
#define DHTTYPE DHT11   // DHT 11
// создание экземпляра объекта DHT
DHT dht(DHTPIN, DHTTYPE);

// создаём объект класса UTFT
// и передаём идентификатор модели дисплея и номера пинов
// к которым подключаются линии SCK, SDI (MOSI), D/C, RESET, CS
UTFT myGLCD(TFT01_22SP, 9, 8, 12, 11, 10);

// объявления встроенных шрифтов
extern uint8_t SmallFont[];
//extern uint8_t BigFont[];
extern uint8_t TestFont[];
extern uint8_t BigMeteoFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t SymbolFont[];

static char temp[5];
static char humi[5];
String sTemp = "";
String sHumi = "";
String sCO2 = "";

String floatToString(float x, byte precision = 2) {
  char tmp[50];
  dtostrf(x, 0, precision, tmp);
  return String(tmp);
}

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
  // запуск датчика DHT
  dht.begin();
  // инициализируем дисплей с вертикальной ориентацией
  myGLCD.InitLCD(0);
  // очищаем экран
  myGLCD.clrScr();
  
}
void loop()
{
  // Wait a few seconds between measurements.
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  float co2 = ccs.geteCO2();


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(co2)) {
    Serial.println(F("Failed to read from sensor!"));
    return;
  }

  myGLCD.setFont(SymbolFont);
  myGLCD.setColor(VGA_AQUA);
  //вывод значка - капля
  myGLCD.print("\x30", 50, 80);
  //myGLCD.print(String(char(1)), 30, 80);

  /*//вывод смайлика
    myGLCD.setColor(VGA_YELLOW);
    myGLCD.print("\x32", 170, 80);
    //  */

  /* //тестовый шрифт
    myGLCD.setFont(TestFont);
    myGLCD.setColor(VGA_WHITE);
    // myGLCD.print("\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x30", 20, 170);
    myGLCD.print("\x30\x31", 20, 170);
    // myGLCD.print(String(char(10)), 180, 80);*/

  Serial.print(F("Humidity: "));
  Serial.print(h);
  // устанавливаем цвет «чернил» для печати и рисования
  myGLCD.setColor(VGA_YELLOW);
  //myGLCD.setFont(BigFont);
  //myGLCD.print(String(char(127)), 10, 10);
  //myGLCD.print("\x7F", 10, 10);
  //myGLCD.print("Humidity, %: ", CENTER, 40);
  myGLCD.setFont(BigMeteoFont);
  //              В   л   а   ж   н   о   с   т   ь   ,       %
  myGLCD.print("\x28\x29\x25\x2A\x2B\x2C\x2D\x26\x2E\x30\x31", CENTER, 40);

  myGLCD.setFont(SevenSegNumFont);
  sHumi = floatToString(h, 0); // conversion call
  myGLCD.setColor(VGA_YELLOW);
  myGLCD.print(sHumi, CENTER, 80); //значеие влажности

  //рисуем смайлик
  int r = 20;
  int x = 170 + r;
  int y = 120 - r;
  myGLCD.setColor(255, 165, 0);
  myGLCD.fillCircle(x, y, r);
  //глаза
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillCircle(x - 3, y - 3, 2);
  myGLCD.fillCircle(x + 3, y - 3, 2);
  //рот
  myGLCD.drawPixel(x - 3, y + 2);
  myGLCD.drawPixel(x - 2, y + 3);
  myGLCD.drawPixel(x - 1, y + 4);
  myGLCD.drawPixel(x, y + 4);
  myGLCD.drawPixel(x + 1, y + 4);
  myGLCD.drawPixel(x + 2, y + 3);
  myGLCD.drawPixel(x + 3, y + 2);

  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  // белая линия по центру экрана
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawLine(1, 160, 240, 160);

//CO2 в центре экрана
myGLCD.setFont(SmallFont);
sCO2 = "eCO2: "+floatToString(co2, 0); // conversion call
myGLCD.setColor(VGA_WHITE);
myGLCD.print(sCO2, CENTER, 170);

  //вывод значка - термометр
  myGLCD.setFont(SymbolFont);
  if (t > 24) {
    myGLCD.setColor(VGA_RED);
  } else {
    myGLCD.setColor(VGA_LIME);
  }
  myGLCD.print("\x31", 50, 250);
  //вывод надписи "Температура,°C"
  myGLCD.setColor(VGA_GREEN);
  myGLCD.setFont(BigMeteoFont);
  //              Т   е   м   п   е   р   а   т   у   р   а   ,   °C
  myGLCD.print("\x20\x21\x22\x23\x21\x24\x25\x26\x27\x24\x25\x30\x2F", CENTER, 200);
  myGLCD.setFont(SevenSegNumFont);
  sTemp = floatToString(t, 0); // conversion call
  myGLCD.print(sTemp, CENTER, 250);


//вывод данных СО2 в сом-порт  
  if (ccs.available())
  {
    //float temp = ccs.calculateTemperature();
    if (!ccs.readData())
    {
      Serial.print("eCO2: ");                                              // Значение уровня eCO2
      Serial.print(co2);
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
