#define BLYNK_TEMPLATE_ID "TemplateID" 
#define BLYNK_TEMPLATE_NAME "Smart Water Planting System" 
char auth[] = "blynk_auth_token"; // the auth code that you got on your gmail 
char ssid[] = "wifi_ssid"; // username or ssid of your WI-FI 
char pass[] = "wifi_password"; // password of your Wi-Fi 
#define relay D2 
#define BLYNK_PRINT Serial 
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h> 
#include <ESP8266HTTPClient.h> 
#include <ArduinoJson.h> 
bool automationEnabled = false; 
bool plantLocationSet = false; 
int soilMoistureLevel = 0; 
int moistureThreshold = 600; 
unsigned long lastWateringTime = 0; 
bool waterMotorState = false; 
bool motor = false; 
WiFiClient wifiClient; 
String apiKey = "Weather_API_KEY";//Weather API Key 
String city = "Warangal"; 
String units = "metric"; 
String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + 
"&appid=" + apiKey + "&units=" + units; 
// Returns false if rainy; true if dry day 

bool checkWeatherData() {
  // Only fetch weather data if connected to Wi-Fi
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Make the HTTP GET request
    http.begin(wifiClient, serverPath);  // Specify the URL
    int httpCode = http.GET();           // Send the request

    // Check if the request was successful
    if (httpCode > 0) {
      String payload = http.getString();  // Get the response payload
      Serial.println("Weather Data: " + payload);  // Print raw JSON data

      // Parse the JSON data
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      // Extract relevant data
      float temperature = doc["main"]["temp"];
      float humidity = doc["main"]["humidity"];
      String weatherDescription = doc["weather"][0]["description"];
      int weatherID = doc["weather"][0]["id"];  // Weather condition ID

      // Print weather data
      Serial.println("Temperature: " + String(temperature) + " °C");
      Serial.println("Humidity: " + String(humidity) + " %");
      Serial.println("Weather: " + weatherDescription);

      // Check if the weather condition indicates rain (Weather ID between 200 and 531)
      if (weatherID >= 200 && weatherID <= 531) {
        Serial.println("Day is rainy no need to give water today");
        // sendMessage("Day is rainy no need to give water today");
        return false;
      } else {
        Serial.println("Day is dry");
      }
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  } else {
    Serial.println("Wi-Fi not connected");
  }
  return true;
  // Return false if rainy; true if dry day
}


void sendMessage(const String& message) {
  Serial.println(message);
  Blynk.virtualWrite(V0, message);
}

void readSoilMoisture() {
  soilMoistureLevel = analogRead(A0); 
  delay(500);
}

void readAndSendSoilMoisture() {
  soilMoistureLevel = analogRead(A0);
  Serial.println(soilMoistureLevel);
  Blynk.virtualWrite(V1, soilMoistureLevel);
}

void giveWater() {
  if (soilMoistureLevel > moistureThreshold) {
    if (checkWeatherData()) {
      giveWaterDirectly();
    } else {
      sendMessage("Day is rainy - no need to water");
    }
  } else {
    sendMessage("Soil moisture sufficient - motor OFF");
  }
}

void giveWaterDirectly() {
  // Serial.println(moistureThreshold);
  // Serial.println(soilMoistureLevel);
  if (soilMoistureLevel > moistureThreshold) { // Adjust threshold based on calibration
    // sendMessage("Water motor ON");
    // Serial.println("Water motor ON");
    if(automationEnabled) {
      if(plantLocationSet) {
        sendMessage("(Automation Enabled) Water motor ON");
      }
      else {
        sendMessage("(Automation Enabled) Day is dry - Water motor ON");
      }
      Blynk.logEvent("(Automation Enabled) Water motor ON");
    }
    else if(!plantLocationSet){
      sendMessage("Day is dry - Water motor ON");
    }
    else {
      sendMessage("Water motor ON");
    }

    int cnt = 0;
    while(soilMoistureLevel > moistureThreshold && cnt<motorTimer) {
      readAndSendSoilMoisture();
      digitalWrite(relay, LOW); // Turn on the water motor 
      waterMotorState = true;
      delay(1000);
      cnt++;
    }
    digitalWrite(relay, HIGH); // Turn off the water motor
    waterMotorState = false;
    sendMessage("Water motor OFF");
    // Serial.println("Water motor OFF");
  } else {
    sendMessage("Soil moisture sufficient-motor OFF");
    // Serial.println("Soil moisture sufficient-motor OFF");
  }
}

void checkPeriodicWatering() {
  unsigned long currentTime = millis();
  // Check if 10 minutes (600000 ms) have passed since last watering
  if (automationEnabled && (lastWateringTime == 0 || currentTime - lastWateringTime >= 60000)) {
    lastWateringTime = currentTime;
    if (plantLocationSet) {
      giveWaterDirectly();
    } else {
      giveWater();
    }
  }
}

BLYNK_WRITE(V2) {
  automationEnabled = param.asInt();
  Serial.print("Automation: ");
  Serial.println(automationEnabled ? "Enabled" : "Disabled");
}

BLYNK_WRITE(V3) {
  plantLocationSet = param.asInt();
  Serial.print("Plant Location: ");
  Serial.println(plantLocationSet ? "Indoor" : "Outdoor");
}

BLYNK_WRITE(V4) {
  bool motor = param.asInt();
  if (motor && automationEnabled) {
    sendMessage("Automation is ON");
  } else if (motor) {
    if (plantLocationSet) {
      giveWaterDirectly();
    } else {
      giveWater();
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode (A0,INPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH); // Default OFF

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);

  while (!Blynk.connected()) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("\nConnected to Blynk server!");
}

void loop() {
  if (Blynk.connected()) {
    Blynk.run();
  } else {
    Serial.println("Blynk connection lost, retrying...");
    Blynk.connect();
  }

  readSoilMoisture();

  if (automationEnabled) {
    checkPeriodicWatering();
  }
