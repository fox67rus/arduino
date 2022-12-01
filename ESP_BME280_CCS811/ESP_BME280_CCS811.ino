#include <ESP8266WiFi.h>                                // Подключаем библиотеку ESP8266WiFi
#include <Wire.h>                                       // Подключаем библиотеку Wire
#include <Adafruit_BME280.h>                            // Подключаем библиотеку Adafruit_BME280
#include <Adafruit_Sensor.h>                            // Подключаем библиотеку Adafruit_Sensor
//#include "Adafruit_CCS811.h"
#include "ccs811.h"
 
#define SEALEVELPRESSURE_HPA (1013.25)                  // Задаем высоту
 
Adafruit_BME280 bme;                                    // Установка связи по интерфейсу I2C
//Adafruit_CCS811 ccs;
CCS811 ccs811(D3); // nWAKE on D3

const char* ssid = "DAP-1360-sw";                       // Название Вашей WiFi сети
const char* password = "88888888";                      // Пароль от Вашей WiFi сети
 
WiFiServer server(80);                                  // Указываем порт Web-сервера
String header;
 
void setup() {
  Serial.begin(115200);                                 // Скорость передачи 115200
  bool status;
                                                       
  if (!bme.begin(0x76)) {                               // Проверка инициализации датчика
    Serial.println("Could not find a valid BME280 sensor, check wiring!"); // Печать, об ошибки инициализации.
    while (1);                                          // Зацикливаем
  }

  Serial.print("setup: ccs811 lib  version: "); Serial.println(CCS811_VERSION);
   
  Wire.begin();                                         // Enable I2C
  Serial.println("Enable I2C");
  ccs811.set_i2cdelay(100); // Needed for ESP8266 because it doesn't handle I2C clock stretch correctly
  bool ok= ccs811.begin();
  if( !ok ) Serial.println("setup: CCS811 begin FAILED");
  
 /*Serial.println("CCS811 test");
  if (!ccs.begin(0x5A))
  {
    Serial.println("Failed to start sensor CCS811! Please check your wiring."); // Если при инициализации на старте датчик не найден,
    while (1);                                                           // в мониторе порта вы получите уведомление об этом.                                                                      
  }*/
  
  Serial.print("Connecting to ");                       // Отправка в Serial port 
  Serial.println(ssid);                                 // Отправка в Serial port 
  WiFi.begin(ssid, password);                           // Подключение к WiFi Сети
  while (WiFi.status() != WL_CONNECTED) {               // Проверка подключения к WiFi сети
    delay(500);                                         // Пауза
    Serial.print(".");                                  // Отправка в Serial port 
  }
 
  Serial.println("");                                   // Отправка в Serial port 
  Serial.println("WiFi connected.");                    // Отправка в Serial port 
  Serial.println("IP address: ");                       // Отправка в Serial port 
  Serial.println(WiFi.localIP());                       // Отправка в Serial port 
  server.begin();                                  
}
 
void loop(){
  WiFiClient client = server.available();               // Получаем данные, посылаемые клиентом 
  
 /* Serial.print("eCO2: ");
  float eCO2 = ccs.geteCO2();
  Serial.print(eCO2);
  Serial.print(" ppm, TVOC: ");      
  float TVOC = ccs.getTVOC();
  Serial.print(TVOC);
  Serial.print(" ppb");*/
  
  if (client) {                                         
    Serial.println("New Client.");                      // Отправка "Новый клиент"
    String currentLine = "";                            // Создаем строку для хранения входящих данных от клиента
    while (client.connected()) {                        // Пока есть соединение с клиентом 
      if (client.available()) {                         // Если клиент активен 
        char c = client.read();                         // Считываем посылаемую информацию в переменную "с"
        Serial.write(c);                                // Отправка в Serial port 
        header += c;
        if (c == '\n') {                                // Вывод HTML страницы 
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");          // Стандартный заголовок HT
            client.println("Content-type:text/html ");
            client.println("Connection: close");        // Соединение будет закрыто после завершения ответа
            client.println("Refresh: 10");              // Автоматическое обновление каждые 10 сек 
            client.println();
            
            client.println("<!DOCTYPE html><html>");    // Веб-страница создается с использованием HTML
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<meta charset='UTF-8'>");   // Делаем русскую кодировку
            client.println("<link rel=\"icon\" href=\"data:,\">");
                     
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial;}");
            client.println("table { border-collapse: collapse; width:40%; margin-left:auto; margin-right:auto; }");
            client.println("th { padding: 12px; background-color: #0043af; color: white; }");
            client.println("tr { border: 1px solid #ddd; padding: 12px; }");
            client.println("tr:hover { background-color: #bcbcbc; }");
            client.println("td { border: none; padding: 12px; }");
            client.println(".sensor { color:white; font-weight: bold; background-color: #bcbcbc; padding: 1px; }");
            
            client.println("</style></head><body><h1>Метеостанция на BME280 и ESP8266</h1>");
            client.println("<table><tr><th>Параметр</th><th>Показания</th></tr>");
            client.println("<tr><td>Температура</td><td><span class=\"sensor\">");
            client.println(bme.readTemperature());
            client.println(" *C</span></td></tr>");    
            client.println("<tr><td>Давление</td><td><span class=\"sensor\">");
            client.println(bme.readPressure() / 100.0F * 0.7501);
            client.println(" мм рт. ст.</span></td></tr>");
            client.println("<tr><td>Влажность</td><td><span class=\"sensor\">");
            client.println(bme.readHumidity());
            client.println(" %</span></td></tr>"); 
            client.println("</body></html>");
            
            client.println();
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
