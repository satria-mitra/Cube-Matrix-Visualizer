#include <Wire.h>
#include <SparkFun_SCD30_Arduino_Library.h> // Include the SCD30 library

SCD30 airSensor;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  airSensor.begin(Wire); // Initialize the SCD30 sensor

  if (airSensor.begin() == false) {
    Serial.println("SCD30 not detected, please check the wiring!");
    while (1); // If the sensor is not detected, halt the program
  }
}

void loop() {
  if (airSensor.dataAvailable()) {
    Serial.print("CO2(ppm): ");
    Serial.print(airSensor.getCO2());

    Serial.print(" Temperature(C): ");
    Serial.print(airSensor.getTemperature(), 1);

    Serial.print(" Humidity(%): ");
    Serial.println(airSensor.getHumidity(), 1);

    delay(2000); // Read the data every 2 seconds
  }
}
