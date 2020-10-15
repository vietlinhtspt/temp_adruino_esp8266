#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

const char* ssid = "Thai Linh"; // Enter your WiFi name
const char* password =  "11111111A"; // Enter WiFi password
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
//const char* mqttUser = "otfxknod";
//const char* mqttPassword = "nSuUc1dDLygF";

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi.. ");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  Serial.println();
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client")) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("testpost", "Hello from ESP8266"); //Topic name
  client.subscribe("testpost");

  Serial.println("Adafruit MLX90614");  
  mlx.begin();  
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
void loop() {
  client.loop();
//  client.publish("testpost", "Hello from ESP8266 in loop"); //Topic name

  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");
  Serial.println();

  const String postData = String(mlx.readAmbientTempC()) + ":" + String(mlx.readObjectTempC());
//  client.publish("testpost", "Hello from ESP8266 in loop"); //Topic name
  char postData_char[20];
  postData.toCharArray(postData_char, sizeof(postData_char));
  client.publish("testpost", postData_char);
  
  delay(2000);
}
