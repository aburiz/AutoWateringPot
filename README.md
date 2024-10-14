
# Auto Watering Plant Pot

This project is an automatic watering system for a plant pot, designed to keep the soil at an ideal moisture level. The system uses a soil moisture sensor to monitor the soil's moisture content, a relay to control a water pump, and a simple low-cost water level sensor to ensure the water reservoir is filled. Additionally, the hardware is housed in a 3D-printed case, making it portable and easy to use.

## Features

- **Soil Moisture Monitoring**: Measures the soil moisture using a soil moisture sensor and converts it into a percentage.
- **Automatic Watering**: Waters the plant when the soil moisture falls below the specified threshold.
- **Water Level Detection**: Alerts the user if the water reservoir is too low using a simple wire-based water sensor.
- **Pump Control**: Uses a relay to control a water pump for a specified duration.
- **3D Printed Housing**: Encases the entire system for easy installation and maintenance.

## Components

- **Arduino-compatible microcontroller** (e.g., ESP32 or similar)
- **Soil Moisture Sensor**
- **Relay Module** for controlling the water pump
- **Water Pump**
- **Wires** for shorting and detecting water levels in the reservoir
- **3D-Printed Housing**
- **Jumper Wires** and connectors

## Code

The code uses the `Arduino` framework and performs the following tasks:

- Reads moisture values from the soil moisture sensor.
- Converts the sensor value into a percentage to represent the soil's moisture level.
- Turns on the water pump if the moisture percentage is below the desired threshold (45% for mint plants).
- Monitors the water level in the reservoir and halts watering if the water is too low.
- Sends status messages over the serial monitor for debugging.

```cpp
#include <Arduino.h>

// Pin Definitions
#define SENSOR_PIN 34
#define RELAY_PIN 5
#define WATER_SENSOR_PIN 14

#define WATERING_TIME 10000

int moistureValue = 0;
int moisturePercentage = 0;
int waterLevel = HIGH;

const int DRY_THRESHOLD = 3500;
const int WET_THRESHOLD = 1000;
const int PUMP_TRIGGER_PERCENTAGE = 45;

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(WATER_SENSOR_PIN, INPUT_PULLUP);
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("Starting Soil Moisture Sensor and Relay Test");
}

void loop() {
  moistureValue = analogRead(SENSOR_PIN);
  moisturePercentage = convertToPercentage(moistureValue);
  Serial.print("Moisture Percentage: ");
  Serial.println(moisturePercentage);
  
  checkWaterLevel();
  
  if (waterLevel == HIGH && moisturePercentage < PUMP_TRIGGER_PERCENTAGE) {
    Serial.println("Soil is dry. Activating pump...");
    pumpOn();
    delay(WATERING_TIME);
    pumpOff();
    delay(2000);
  } else if (moisturePercentage >= PUMP_TRIGGER_PERCENTAGE) {
    Serial.println("Soil is sufficiently moist. No need to water.");
  }

  delay(4000);
}

int convertToPercentage(int value) {
  return (int)(((DRY_THRESHOLD - value) * 1.0) / (DRY_THRESHOLD - WET_THRESHOLD) * 100.0);
}

void checkWaterLevel() {
  waterLevel = digitalRead(WATER_SENSOR_PIN);
  if (waterLevel == LOW) {
    Serial.println("WARNING! Water Refill Needed!");
  } else {
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
```

## 3D-Printed Housing

The housing for this project was designed to fit the components snugly and securely. It features compartments for the soil moisture sensor, relay module, and water pump tubing. You can find the `.STL` files in the `3d-prints/` folder.

## Usage

1. Clone the repository and upload the code to your Arduino-compatible microcontroller.
2. Assemble the components as per the schematic.
3. Place the soil moisture sensor in the soil, ensuring good contact.
4. Fill the water reservoir and ensure the water level sensor wires are positioned correctly.
5. Run the system and monitor the serial output to check the sensor readings and pump status.

## Customization

- You can adjust the moisture threshold, pump running time, and other variables in the code to suit different types of plants or environmental conditions.
- Modify the 3D-printed case to fit your specific components or aesthetic preferences.

