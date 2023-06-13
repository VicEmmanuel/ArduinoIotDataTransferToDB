#include <ArduinoHttpClient.h> 
#include <WiFi101.h> 
#include <DHT.h> 
#include <SPI.h> 
#include <ArduinoJson.h> 
 
/////// Please enter your sensitive data in the Secret tab/arduino_secrets.h 
/////// WiFi Settings /////// 
const char* ssid = "FUTO-SPESSE"; 
const char* password = "Password"; 
 
#define DHTPIN 2 
#define DHTTYPE DHT11 
 
char serverAddress[] = "apiv1.i-edge.tech";  // API address 
int port = 80;  // Use port 443 for HTTPS 
 
DHT dht(DHTPIN, DHTTYPE); 
 
WiFiClient wifi; 
HttpClient client = HttpClient(wifi, serverAddress, port); 
int status = WL_IDLE_STATUS; 
 
void setup() { 
  Serial.begin(115200); 
  dht.begin(); 
   
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to Network named: "); 
    Serial.println(ssid);                   // print the network name (SSID); 
     
    // Connect to WPA/WPA2 network: 
    status = WiFi.begin(ssid, password); 
  } 
   
  // print the SSID of the network you're attached to: 
  Serial.print("SSID: "); 
  Serial.println(WiFi.SSID()); 
   
  // print your WiFi shield's IP address: 
  IPAddress ip = WiFi.localIP(); 
  Serial.print("IP Address: "); 
  Serial.println(ip); 
} 
 
void loop() { 
  int temperature = dht.readTemperature(); 
  float humidity = dht.readHumidity(); 
 
  Serial.println(temperature); 
  Serial.println(humidity); 
  Serial.println("Making POST request"); 
   
  // Create JSON object 
  StaticJsonDocument<200> jsonDoc; 
  jsonDoc["value"] = temperature; 
//  jsonDoc["humidity"] = humidity; 
   
  // Serialize JSON to a string 
  String postData; 
  serializeJson(jsonDoc, postData); 
   
  client.beginRequest(); 
  client.post("/api/numbers"); // Replace with your API endpoint 
  //client.sendBasicAuth("test@gmail.com", "12345678"); 
  client.sendHeader("Content-Type", "application/json"); 
  client.sendHeader("Content-Length", postData.length()); 
  client.sendHeader("Authorization", "Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJhdWQiOiI5OTY2NWE4NS1hNTI4LTQxZGEtOWY2OS1lMTQxMTI0ZjNkYzAiLCJqdGkiOiJkZWFjMzcwMmQ2MTRkZDljNTY3N2JkNjk1NjA0NWFlM2VmOGY1MjFhNjRhYzkwOGJhYTg0ZGQ0MGJmODk2NzdiNjgzOGIwNzc1MGUwYjJjZSIsImlhdCI6MTY4NjY0ODg3MC42NjA3ODgwNTkyMzQ2MTkxNDA2MjUsIm5iZiI6MTY4NjY0ODg3MC42NjA3OTA5MjAyNTc1NjgzNTkzNzUsImV4cCI6MTcwMjQ2MDA3MC42NTQ3MzM4OTYyNTU0OTMxNjQwNjI1LCJzdWIiOiIxIiwic2NvcGVzIjpbXX0.b-ZT_p1vRypZMmWIIJXKZoyvI8KB6IKZ2fVrNL9nB0cI3sXIG8VUsXsIVg4xr2GoeYRPoSy9zvYYI162gw4zFOWxjdKbO27KTzHV9fiT7vT8BLCpWoKIVfGVzLgCpGF6LGSvFTNDiLslkqJgVeW11vAd6nGlPrukxSXwalreZpuCmaVGuVcMy-wH0P7YmaN7FNaDb1SF-LVnKvTBBW6s8qQvTbZhZFbny9EAiEDl-MtoEnd6GjKYjKT2Yn7EZzC4QjQFasf-5U21AntrmsZF9Q2Yl0ZqGsLmcvMsR7bIJ-8nwxMpJh6oDjjMNGEUcvvQz0q01q3pPp4s07zveBkHgeO_wLVbwukyckfn23-UG6bx-4su68hTUwrFo00wqC9pKnkP6dYLa9_z0jfCZjKH877navUipHRh5GxZabH7X5lpH8avcZlyB_vZMSt-lefT4YV1r7rxEvXnRly6n_ZeZFKxdqA7KR2q5LjKg8MOxXtv6_gOdt021zsh8Vl-oRXpeLaYPzc1v2rcXZ9JsAnczuAY3Bul9wrDPlGN5wHX_wPjPQnuX_wV8vGj6dX81hQ9pHCYQ1x7hUwLBlwoQ2kRGPFRuWuXqiEMTgVjOzF9RW2-2czXydcYFcpKaNPN20g3YH-QmpRaERfi0HN13NZuL0Ciqa-xp6i-EmTApDQziuY"); 
 
  client.beginBody(); 
  client.print(postData); 
  client.endRequest(); 
   
  // Read the status code and body of the response 
  int statusCode = client.responseStatusCode(); 
  String response = client.responseBody(); 
   
  Serial.print("Status code: "); 
  Serial.println(statusCode); 
  Serial.print("Response: "); 
  Serial.println(response); 
   
  Serial.println("Wait five seconds"); 
  delay(2000); 
}
