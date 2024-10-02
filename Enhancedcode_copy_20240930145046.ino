#include <Arduino.h>

// Pin Definitions
#define SENSOR_PIN 34 // Pin connected to the soil moisture sensor
#define RELAY_PIN 5 // Pin connected to the relay which controls the pump
#define WATER_SENSOR_PIN 14 // Pin connected to the wire in the reservoir that works as a low water level indicator

// Pump running time (in milliseconds)
#define WATERING_TIME 10000 // Run pump for 10 seconds when watering

// Variables
int moistureValue = 0; // Initialization of the variable
int moisturePercentage = 0; // Declaring and initializing a variable to calculate moisture percentage
int waterLevel = HIGH; // Initializing the state of the water level sensor that we made by shorting a wire at a certain level when it comes in touch with water

// Declare the functions
void pumpOn(); // Function to turn the pump on
void pumpOff(); // Function to turn the pump off
void checkWaterLevel(); // Function to check when water reservoir level is too low
int convertToPercentage(int Value);  // Function to convert the sensor value to moisture percentage

// Constant values
const int DRY_THRESHOLD = 3500; // Value at which the soil is considered dry (found during calibration)
const int WET_THRESHOLD = 1000; // Value at which the soil is considered wet (found during calibration)
const int PUMP_TRIGGER_PERCENTAGE = 45; // The pump triggers and turns on when the moisture level falls below this value

// We are working with mint leaves and the ideal soil moisture percentage for mint growth is 40%-50%, thus we are using 45% as the ideal moisture percentage and the trigger value

void setup()  {

  // Initialize Serial Monitor for debugging
  Serial.begin(115200);
  
  // This initializes the pin with its role (input/output)
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(WATER_SENSOR_PIN, INPUT_PULLUP);

  // Ensure the pump is off initially
  digitalWrite(RELAY_PIN, HIGH); 

  // Display a startup message
  Serial.println("Starting Soil Moisture Sensor and Relay Test");
}

void loop() {
  
   // Read the moisture level from the sensor
  moistureValue = analogRead(SENSOR_PIN);

  // Convert the sensor moisture value to moisture percentage
  moisturePercentage = convertToPercentage(moistureValue);

  Serial.print("Moisture Percentage: ");
  Serial.println(moisturePercentage); // Sends current moisture percentage over USB

  checkWaterLevel();

  if (waterLevel == HIGH) { // This condition is here to ensure that the system will not run if the water level is too low as a safety measure for the water pump
    if (moisturePercentage < PUMP_TRIGGER_PERCENTAGE) { // If moisture level is below the threshold, the soil is dry, so we will water the soil
      Serial.println("Soil is dry. Activating pump...");
      pumpOn(); // Turn on the relay to start the pump
      delay(WATERING_TIME); // Run the pump and water the soil for the specified time 
      pumpOff(); // Turn off the relay to stop the pump
      delay(2000); // to give the water & moisture sensor some time to balance itself before we check moisture
    } else {
        Serial.println("Soil is sufficiently moist. No need to water."); // Display while moisture level is above the threshold
    }
  }
  delay(4000); // Wait for a few seconds before checking again
}

int convertToPercentage(int Value) {
  float fpercentage = ((DRY_THRESHOLD - Value)*1.0)/((DRY_THRESHOLD - WET_THRESHOLD)*1.0)*100.0; // Equation to convert the value into percentage
  int percentage = (int) fpercentage; // Typecasting to integer values of percentage
  return percentage;
}

void checkWaterLevel() {
  waterLevel = digitalRead(WATER_SENSOR_PIN);
  if (waterLevel == LOW) {
    Serial.println("WARNING! Water Refill Needed!");
  } else if (waterLevel == HIGH) {
    Serial.println("Water Level is okay :)");
  }
}

void pumpOn() {
  digitalWrite(RELAY_PIN, LOW); 
  Serial.println("The pump turned ON"); 
}

void pumpOff() {
  digitalWrite(RELAY_PIN, HIGH); 
  Serial.println("The pump turned OFF");
}

