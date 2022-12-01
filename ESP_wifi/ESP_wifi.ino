#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"
// Раскомментируйте одну из строк ниже для того типа DHT датчика, который вы используете!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
/*Укажите SSID & пароль*/
const char* ssid = "DAP-1360-sw";  // Enter SSID here
const char* password = "88888888";  //Enter Password here
//реле
const int relPin = D1;
//server
ESP8266WebServer server(80);
// DHT датчик
uint8_t DHTPin = D3; 
               
// Инициализация датчика DHT
DHT dht(DHTPin, DHTTYPE);                
float Temperature;
float Humidity;
 
void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(DHTPin, INPUT);
  dht.begin();              
  Serial.println("Connecting to ");
  Serial.println(ssid);
  // подключаемся к локальной сети Wi-Fi
  WiFi.begin(ssid, password);
  // проверка подключения Wi-Fi 
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
  pinMode(relPin, OUTPUT); 
}
void loop() {  
   // digitalWrite(relPin, HIGH);
  //  delay(5000);
  //  digitalWrite(relPin, LOW);
   // delay(10000); 
  server.handleClient();
  
}
void handle_OnConnect() {
 Temperature = dht.readTemperature(); // Получает значения температуры
  Humidity = dht.readHumidity(); // Получает значения влажности
  server.send(200, "text/html", SendHTML(Temperature,Humidity)); 
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
String SendHTML(float Temperaturestat,float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Weather Report</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP8266 NodeMCU Weather Report</h1>\n";  
  ptr +="<p>Temperature: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Humidity: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";  
  ptr +="</div>\n";
  //Управление реле
  ptr +="<div id=\"switcher\">\n";
  ptr +="<h1>Switcher</h1>\n";
  ptr +="<p>Status";
  ptr +="<p>";
  ptr +="<a href=\'?pin=ON\'><button>Switch ON</button></a>";
  ptr +="<a href=\'?pin=OFF\'><button>Switch OFF</button></a>";
  ptr +="</p>";  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

//теперь опрос нажатых кнопок
//        local _on,_off = "",""
//        if(_GET.pin == "ON")then
//              gpio.write(relPin, gpio.LOW);
//        elseif(_GET.pin == "OFF")then
//              gpio.write(relPin, gpio.HIGH);
