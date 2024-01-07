// Wi-Fi library for connecting to a wireless network
#include <WiFi.h>
#include <WiFiClient.h>

// Library for controlling I2C-based LCD displays
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Library for HD44780-compatible LCDs
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

// Library for interfacing with DHT sensors
#include <DHT.h>

// Blynk app template details
#define BLYNK_TEMPLATE_ID           "TMPL5Scu-z8wi"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "eiIL-ZpBVYsZZdvM44AGCsxhBcTToHB8"
#include <BlynkSimpleEsp32.h>

// Wi-Fi Login
char ssid[] = "KentTest213";
char pass[] = "12345666";

// LCD config
hd44780_I2Cexp lcd;

// Pin definitions
const int soilSensorPin = A0;  // Analog pin connected to SIG pin of the soil sensor
const int dhtPin = 14;         // Replace with the GPIO pin your DHT sensor is connected to
const int relayPin = 18;       // Replace with the GPIO pin your relay is connected to

// DHT sensor setup
DHT dht(dhtPin, DHT11);

void setup() {
  // Initialization serial communication
  Serial.begin(115200);
  lcd.begin(16, 2);
  dht.begin();
  // Connect to Wi-Fi and Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // Set relay pin as output and turn it off initially
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
}

void loop() {
  Blynk.run();
  
  // Read sensor values
  int soilMoistureReading = analogRead(soilSensorPin);
  int moisturePercentage = map(soilMoistureReading, 0, 4095, 0, 100); // Convert to percentage
  float temperatureReading = dht.readTemperature();
  float humidityReading = dht.readHumidity();

  // Print sensor readings to Serial
  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercentage);
  Serial.print("% | ");
  Serial.println("");
  Serial.print("Temperature: ");
  Serial.print(temperatureReading);
  Serial.print("°C | Humidity: ");
  Serial.print(humidityReading);
  Serial.println("");
  Serial.print("% | Relay Status: ");
  
  // Control relay based on soil moisture
  if (moisturePercentage < 40) {
    digitalWrite(relayPin, HIGH);  // Turn on the relay when moisture is below 40%
  } else {
    digitalWrite(relayPin, LOW);   // Turn off the relay otherwise
  }

  // Combine sensor values for Blynk visualization
  String combinedSensorValues = "Moisture: " + String(moisturePercentage) + "%\nTemp: " + String(temperatureReading) + "°C\nHum: " + String(humidityReading) + "%";
  Blynk.virtualWrite(V3, combinedSensorValues);

  // Update LCD displays
  lcd.print("Moisture Level: ");
  lcd.print(moisturePercentage);
  lcd.print("%");
  delay(2000);
  lcd.clear();
  lcd.print("Humidity: ");
  lcd.print(humidityReading);
  delay(2000);
  lcd.clear();
  lcd.print("Temperature: ");
  lcd.setCursor(0, 1);
  lcd.print(temperatureReading);
  lcd.print("C ");
  delay(2000);
  lcd.clear();
}
