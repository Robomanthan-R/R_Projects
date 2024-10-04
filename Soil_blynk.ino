// Blynk template information
#define BLYNK_TEMPLATE_ID "TMPL33QYJj3Ss"
#define BLYNK_TEMPLATE_NAME "Plant watering system"

// Include the library files
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define moisturePin 33   // Pin connected to the soil moisture sensor
#define relay 2          // Relay controlling the AC light

// Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

BlynkTimer timer;

// Enter your Auth token
char auth[] = "2V4ug0ZLzSvaGuPDHaTmG-3Q108UboHm";

// Enter your WIFI SSID and password
char ssid[] = "Robomanthan4g";
char pass[] = "12345678";

// Function to get soil moisture level
int getSoilMoisture() {
  int moistureValue = analogRead(moisturePin);  // Read the sensor value
  moistureValue = map(moistureValue, 0, 4095, 0, 100);  // Map to percentage (0-100%)
  return moistureValue;
}

void setup() {
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  lcd.init();
  lcd.backlight();
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);  // Relay initially OFF (AC light OFF)
  
  pinMode(moisturePin, INPUT);  // Set soil moisture pin as input

  lcd.setCursor(1, 0);
  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(200);
  }
  lcd.clear();
}

// Check the soil moisture level and control the relay
void moistureCheck() {
  int moistureValue = getSoilMoisture();
  Blynk.virtualWrite(V0, moistureValue);  // Send moisture level to Blynk app
  Serial.print("Soil Moisture: ");
  Serial.print(moistureValue);
  Serial.println(" %");

  lcd.setCursor(0, 0);
  lcd.print("Moisture : ");
  lcd.print(moistureValue);
  lcd.print(" %  ");

  // If the soil moisture is less than 30%, turn ON the relay (AC light ON)
  if (moistureValue < 30) {
    digitalWrite(relay, LOW); // Relay ON (AC light ON)
    lcd.setCursor(0, 1);
    lcd.print("Light is ON ");
  } else {
    digitalWrite(relay, HIGH); // Relay OFF (AC light OFF)
    lcd.setCursor(0, 1);
    lcd.print("Light is OFF");
  }
}

// Get the button value from Blynk (optional)
BLYNK_WRITE(V1) {
  bool Relay = param.asInt();
  if (Relay == 1) {
    digitalWrite(relay, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Light is ON ");
  } else {
    digitalWrite(relay, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Light is OFF");
  }
}

void loop() {
  moistureCheck();  // Check soil moisture and control the relay
  Blynk.run();  // Run the Blynk library
  delay(200);
}
