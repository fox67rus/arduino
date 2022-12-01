#include <ESP8266WiFi.h>
#include <DallasTemperature.h>
#include <OneWire.h>

//подключаемся к сети
const char* ssid = "DAP-1360-sw";//type your ssid
const char* password = "88888888";//type your password
const int relPin = D1; // GPIO5 of ESP8266
// веб-сервер на порте 80: 
WiFiServer server(80);

// Провод данных подключен к контакту D2 на ESP8266 12-E (GPIO4)
#define ONE_WIRE_BUS 4

// создаем экземпляр класса oneWire; с его помощью 
// можно коммуницировать с любыми девайсами, работающими 
// через интерфейс 1-Wire, а не только с температурными датчиками
// от компании Maxim/Dallas:
OneWire oneWire(ONE_WIRE_BUS);

// передаем объект oneWire объекту DS18B20: 
DallasTemperature DS18B20(&oneWire);
char temperatureCString[6];
char temperatureFString[6];


void setup() {
Serial.begin(115200);
delay(10);
pinMode(relPin, OUTPUT); 
digitalWrite(relPin, HIGH);

DS18B20.begin(); // по умолчанию разрешение датчика – 9-битное;
                   // если у вас какие-то проблемы, его имеет смысл
                   // поднять до 12 бит; если увеличить задержку, 
                   // это даст датчику больше времени на обработку
                   // температурных данных

// Connect to WiFi network
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.hostname("WiFiRozetka");
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");

// Start the server
server.begin();
Serial.println("Server started");
delay(5000);
// Print the IP address
Serial.print("Use this URL to connect: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");
}

void loop() {
// Check if a client has connected
WiFiClient client = server.available();
if (!client) {
return;
}

// Wait until the client sends some data
Serial.println("new client");
while(!client.available()){
delay(1);
}

// Read the first line of the request
String request = client.readStringUntil('\r');
Serial.println(request);
client.flush();

      // Match the request
      // запрашиваем sensor.requestTemperatures() для получения глобальной температуры
      // запрос всех устройств на шине
Serial.print(" Запрашиваем температуру...");
DS18B20.requestTemperatures(); // Отправляем команды для получения температуры
Serial.println("DONE");
Serial.print("Температура: ");
  float Temp = DS18B20.getTempCByIndex(0);// Почему "byIndex"? 
   // У вас может быть несколько IC на одной шине. 
    // 0 относится к первой IC
  Serial.println(Temp); 
   delay(1000);
int value = HIGH;
if (request.indexOf("/Relay=OFF") != -1) {
digitalWrite(relPin, HIGH);
value = HIGH;
} 
if (request.indexOf("/Relay=ON") != -1){
digitalWrite(relPin, LOW);
value = LOW;
}
// Return the response
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(""); //  do not forget this one
client.println("<!DOCTYPE HTML>");
client.println("<html>");
//client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">");
//client.println("<meta http-equiv=\"refresh\" content=\"10\" >");
client.print("<style>");
client.print("html {font-family: Helvetica; font-size: 2em; display: inline-block; margin: 0px auto; text-align: center;}"); 
client.print("body {background:#e9fbe8; margin-top: 50px;} h1{color: #444444; margin: 50px auto 30px;}");
client.print(".temperature{color: #f39c12;}");
client.print(".temperature-icon{background-color: #f39c12;}");
client.print(".temperature-text{font-size: 3em;}");
client.print(".superscript{font-size: 0.5em;font-weight: 600;position: absolute;}");
client.print(".data{padding: 5%;}");
client.print("button {font-size: 4em; border-radius: 30%; transition-duration: 0.4s;}");
client.print(".but_on {background-color: white; color: black; border-color: #00ff00;}");
client.print(".but_on:hover {background-color: #c4ffd2; color: white;}");
client.print(".but_off {background-color: white; color: black; border-color: #ff0000;}");
client.print(".but_off:hover {background-color: #ffc4c4; color: white;}");
client.println("</style>");
client.println("<title>Wifi Rozetka</title>");
client.println("</head>");
client.println("<body>");
client.println("<h1>WiFi Rozetka</h1>");

client.print("Status now: ");
if(value == HIGH) {
client.print("Off<br>");  
} else {
client.print("On<br>");
}
client.println("<div class=\"data\">");
client.println("<a href=\"/Relay=ON\"><button class=\"but_on\">ON</button></a>");
client.println("<a href=\"/Relay=OFF\"><button class=\"but_off\">OFF</button></a></div>");

client.println("<h2>Temperature</h2>");
client.print("<div class=\"temperature-icon\">");
client.print("<svg version=\"1.1\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"");
client.print("width=\"9.915px\" height=\"22px\" viewBox=\"0 0 9.915 22\" enable-background=\"new 0 0 9.915 22\" xml:space=\"preserve\">");
client.print("<path fill=\"#FFFFFF\" d=\"M3.498,0.53c0.377-0.331,0.877-0.501,1.374-0.527C5.697-0.04,6.522,0.421,6.924,1.142");
client.print("c0.237,0.399,0.315,0.871,0.311,1.33C7.229,5.856,7.245,9.24,7.227,12.625c1.019,0.539,1.855,1.424,2.301,2.491");
client.print("c0.491,1.163,0.518,2.514,0.062,3.693c-0.414,1.102-1.24,2.038-2.276,2.594c-1.056,0.583-2.331,0.743-3.501,0.463");
client.print("c-1.417-0.323-2.659-1.314-3.3-2.617C0.014,18.26-0.115,17.104,0.1,16.022c0.296-1.443,1.274-2.717,2.58-3.394");
client.print("c0.013-3.44,0-6.881,0.007-10.322C2.674,1.634,2.974,0.955,3.498,0.53z\"/>");
client.print("</svg>");
client.print("</div>");

client.println("<div class=\"temperature temperature-text\">");
client.print(Temp);
client.print("<span class=\"superscript\">&deg;C</span></div>");
client.println("</body></html>");
delay(1);
Serial.println("Client disconnected");
Serial.println("");
}
