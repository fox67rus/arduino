#include <ESP8266WiFi.h>                                // Подключаем библиотеку ESP8266WiFi
#include <Wire.h>                                       // Подключаем библиотеку Wire
#include "Adafruit_CCS811.h"
 

 
Adafruit_CCS811 ccs;                                     // Установка связи по интерфейсу I2C
 
const char* ssid = "DAP-1360-sw";                       // Название Вашей WiFi сети
const char* password = "88888888";                      // Пароль от Вашей WiFi сети
 
WiFiServer server(80);                                  // Указываем порт Web-сервера
String header;
 
void setup() {
  Serial.begin(115200);                                 // Скорость передачи 115200
  bool status;
                                                       
  if (!ccs.begin()) {                               // Проверка инициализации датчика
    Serial.println("Could not find a valid CCS811 sensor, check wiring!"); // Печать, об ошибки инициализации.
    while (1);                                          // Зацикливаем
  }
 
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
           /* client.println("<tr><td>Температура</td><td><span class=\"sensor\">");
            client.println(bme.readTemperature());
            client.println(" *C</span></td></tr>");    
            client.println("<tr><td>Давление</td><td><span class=\"sensor\">");
            client.println(bme.readPressure() / 100.0F * 0.7501);
            client.println(" мм рт. ст.</span></td></tr>");
            client.println("<tr><td>Влажность</td><td><span class=\"sensor\">");
            client.println(bme.readHumidity());
            client.println(" %</span></td></tr>"); */
            client.println("<tr><td>eCO2</td><td><span class=\"sensor\">");
            client.println(ccs.geteCO2());
            client.println(" ppm</span></td></tr>");
            client.println("<tr><td>TVOC</td><td><span class=\"sensor\">");
            client.println(ccs.getTVOC());
            client.println(" ppb</span></td></tr>");
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
