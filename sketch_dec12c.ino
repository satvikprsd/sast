#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>

//-----------------------------------------------------------------------------------------------------
//                                               DEFINES
//-----------------------------------------------------------------------------------------------------
#define DHTPIN  4                                // DHT output pin
#define DHTTYPE DHT11                              // DHT type (11, 21, 22)
#define AIR 34  
//-----------------------------------------------------------------------------------------------------
//                                              VARIABLES
//-----------------------------------------------------------------------------------------------------
const int led_pin = 2;                             // Onboard LED for ESP32

//-----------------------------------------------------------------------------------------------------
//                                           DATA STRUCTURES
//-----------------------------------------------------------------------------------------------------
struct package {
    int air;
    float temperature;
    float humidity;
    float pressure;
    float altitude;
    float accex, accey, accez;
    float gyrx, gyry, gyrz;
    int hall_value;
};

typedef struct package Package;
Package data;

//-----------------------------------------------------------------------------------------------------
//                                            LIBRARY CALLS
//-----------------------------------------------------------------------------------------------------
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
Adafruit_MPU6050 mpu;

//-----------------------------------------------------------------------------------------------------
//                                                SETUP
//-----------------------------------------------------------------------------------------------------
void setup() {
    Serial.begin(115200);                          // Start serial monitor
    Wire.begin();                                  // Initialize I2C
    pinMode(led_pin, OUTPUT);
    pinMode(AIR,INPUT); 
    // Initialize sensors
    dht.begin();
    bmp.begin();
    mpu.begin();

    Serial.println("Setup complete");
}

//-----------------------------------------------------------------------------------------------------
//                                              MAIN LOOP
//-----------------------------------------------------------------------------------------------------
void loop() {
    digitalWrite(led_pin, HIGH);                  // Turn on LED to indicate reading data
    readSensor();                                 // Read sensor data
    outputData();                                 // Output data via serial
    digitalWrite(led_pin, LOW);                   // Turn off LED
                                    // Delay before next iteration
}

//-----------------------------------------------------------------------------------------------------
//                                              FUNCTIONS
//-----------------------------------------------------------------------------------------------------
void readSensor() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    data.air  = analogRead(AIR); 
    data.humidity = dht.readHumidity();
    data.temperature = dht.readTemperature();
    data.pressure = bmp.readPressure();
    data.altitude = bmp.readAltitude();
    data.accex = a.acceleration.x;
    data.accey = a.acceleration.y;
    data.accez = a.acceleration.z;
    data.gyrx = g.gyro.x;
    data.gyry = g.gyro.y;
    data.gyrz = g.gyro.z;

    // Read the Hall sensor value from the ESP32
    data.hall_value = hallRead();
}

void outputData() {
    Serial.print(data.temperature);
    Serial.print(","); 
    Serial.print(data.humidity);
    Serial.print(","); 
    Serial.print(data.pressure);
    Serial.print(","); 
    Serial.print(data.altitude);
    Serial.print(",");
    Serial.print(data.hall_value); 
    Serial.print(",");
    Serial.print(data.air); 
    Serial.print(",");
    Serial.print(data.gyry); 
    Serial.print(",");
    Serial.print(data.gyrx); 
    Serial.print(",");
    Serial.println(data.gyrz);
}
