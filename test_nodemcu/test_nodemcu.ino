
void setup()
{
  pinMode(D0, OUTPUT);
  Serial.begin(115200); 
  
}
void loop()
{
  digitalWrite(D0, HIGH);
  Serial.println("ON");   
  delay(500);
  digitalWrite(D0, LOW);
  Serial.println("OFF"); 
  delay(500);
}
