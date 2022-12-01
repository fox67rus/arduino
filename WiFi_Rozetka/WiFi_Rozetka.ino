#include <ESP8266WiFi.h>

const char* ssid = "DAP-1360-sw";//type your ssid
const char* password = "88888888";//type your password
const int relPin = D1; // GPIO5 of ESP8266
WiFiServer server(80);//Service Port

void setup() {
Serial.begin(115200);
delay(10);
pinMode(relPin, OUTPUT); 
digitalWrite(relPin, HIGH);

// Connect to WiFi network
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

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
client.print("<style>");
client.print("html {font-family: Helvetica; font-size: 2em; display: inline-block; margin: 0px auto; text-align: center;}"); 
client.print("body {background:#e9fbe8; margin-top: 50px;} h1{color: #444444; margin: 50px auto 30px;}");
client.print("button {font-size: 2em;}");
client.println("</style>");
client.print("<h1>WiFi Rozetka</h1>");
client.print("Relay pin is now: ");
if(value == HIGH) {
client.print("Off<br>");  
} else {
client.print("On<br>");
}
client.println("<br><br>");
client.println("<a href=\"/Relay=ON\"><button>Switch ON</button></a><br><br>");
client.println("<a href=\"/Relay=OFF\"><button>Switch OFF</button></a><br>");
client.println("</html>");
delay(1);
Serial.println("Client disconnected");
Serial.println("");
}
