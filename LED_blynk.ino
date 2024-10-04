// Blynk template information
#define BLYNK_TEMPLATE_ID "TMPL33QYJj3Ss"
#define BLYNK_TEMPLATE_NAME "Plant watering system"

// Include the necessary libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Define the pin for the light (LED)
#define lightPin 2

// Enter your Blynk Auth token
char auth[] = "2V4ug0ZLzSvaGuPDHaTmG-3Q108UboHm";

// Enter your WiFi credentials
char ssid[] = "Robomanthan4g";
char pass[] = "12345678";

void setup() {
  // Start the serial communication for debugging purposes
  Serial.begin(115200);
  
  // Set the light pin as output
  pinMode(lightPin, OUTPUT);
  
  // Initialize the light to OFF state
  digitalWrite(lightPin, LOW);
  
  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);
}

BLYNK_WRITE(V1) {
  // This function is called whenever there is a change on Virtual Pin V1
  int lightState = param.asInt();  // Get the state of the button on Blynk app
  
  if (lightState == 1) {
    digitalWrite(lightPin, HIGH);  // Turn the light ON
    Serial.println("Light ON");
  } else {
    digitalWrite(lightPin, LOW);   // Turn the light OFF
    Serial.println("Light OFF");
  }
}

void loop() {
  Blynk.run();  // Run the Blynk library
}
