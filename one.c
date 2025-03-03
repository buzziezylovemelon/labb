#include <WiFi.h>
#include "ThingSpeak.h" // Always include ThingSpeak after other headers

WiFiClient client;
const int LED = 32;
const char *ssid = "Raul";
const char *password = "buzziezy071148";
unsigned long myChannelNumber = 2862160;
const char *myWriteAPIKey = "6U7GPCIRNAKKHHR1";

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not CONNECTED");
    delay(500);
  }
  
  digitalWrite(LED, LOW);
  Serial.println("WiFi CONNECTED");

  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED, !digitalRead(LED));    

    // Use GPIO34 for analog input (ESP32 doesn't have A0 like Arduino UNO)
    ThingSpeak.setField(1, analogRead(34));
    ThingSpeak.setField(2, hallRead());    

    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    
    if (x == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  } else {
    digitalWrite(LED, HIGH);
  }
  
  delay(15000);
}