/*
  Arduino-Powered Thermochromic Printer
  Description:
  - Spins a motor when the user enters "start" in the serial console.
  - Adjusts the heater temperature based on user input commands.
  - Uses a temperature sensor to maintain the desired temperature.
*/

#include <PID_v1.h>

// ===== Pin Definitions =====
#define MOTOR_IN1_PIN    8
#define MOTOR_IN2_PIN    9
#define MOTOR_ENABLE_PIN 10 // PWM-capable pin
#define HEATER_PIN       5  // PWM-capable pin
#define TEMP_SENSOR_PIN  A0

// ===== PID Control Variables =====
double Setpoint, Input, Output;

// PID tuning parameters
double Kp = 2.0, Ki = 5.0, Kd = 1.0;

// Initialize PID controller
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// ===== Temperature Variables =====
const float referenceVoltage = 5.0;
const int adcResolution = 1023;
const float tempOffset = 0.5; // Offset for TMP36
float currentTemp = 0.0;
float desiredTemp = 100.0; // Default temperature in Celsius

// ===== Motor Control Variables =====
const unsigned long motorRunDuration = 5000; // Motor runs for 5 seconds
bool motorRunning = false;
unsigned long motorStartTime = 0;

// ===== Function Prototypes =====
float readTemperature();
void spinMotor();
void stopMotor();

void setup() {
    // Initialize Serial Communication
    Serial.begin(9600);
    Serial.println("Thermochromic Printer Initialized.");
    Serial.println("Enter 'start' to spin the motor or 'temp <value>' to set temperature.");

    // Initialize Motor Control Pins
    pinMode(MOTOR_IN1_PIN, OUTPUT);
    pinMode(MOTOR_IN2_PIN, OUTPUT);
    pinMode(MOTOR_ENABLE_PIN, OUTPUT);
    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, LOW);
    analogWrite(MOTOR_ENABLE_PIN, 0); // Motor off initially

    // Initialize Heater Control Pin
    pinMode(HEATER_PIN, OUTPUT);
    analogWrite(HEATER_PIN, 0); // Heater off initially

    // Initialize Temperature Sensor Pin
    pinMode(TEMP_SENSOR_PIN, INPUT);

    // Initialize PID Controller
    Setpoint = desiredTemp;
    myPID.SetMode(AUTOMATIC);
    myPID.SetOutputLimits(0, 255); // PWM range
}

void loop() {
    // Handle Serial Input
    if (Serial.available() > 0) {
        String inputStr = Serial.readStringUntil('\n');
        inputStr.trim(); // Remove any trailing newline or spaces

        // Parse Commands
        if (inputStr.equalsIgnoreCase("start")) {
            Serial.println("Starting motor...");
            spinMotor();
        }
        else if (inputStr.startsWith("temp")) {
            int spaceIndex = inputStr.indexOf(' ');
            if (spaceIndex != -1) {
                String tempValueStr = inputStr.substring(spaceIndex + 1);
                desiredTemp = tempValueStr.toFloat();
                Setpoint = desiredTemp;
                Serial.print("Desired temperature set to: ");
                Serial.print(desiredTemp);
                Serial.println(" °C");
            }
            else {
                Serial.println("Invalid temperature command. Use 'temp <value>'.");
            }
        }
        else {
            Serial.println("Unknown command. Use 'start' or 'temp <value>'.");
        }
    }

    // Read Current Temperature
    currentTemp = readTemperature();
    Input = currentTemp;

    // Compute PID Output
    myPID.Compute();

    // Control Heater
    analogWrite(HEATER_PIN, (int)Output);

    // Display Temperature Info
    Serial.print("Current Temperature: ");
    Serial.print(currentTemp);
    Serial.print(" °C | Desired Temperature: ");
    Serial.print(desiredTemp);
    Serial.println(" °C");

    // Handle Motor Running Duration
    if (motorRunning) {
        if (millis() - motorStartTime >= motorRunDuration) {
            stopMotor();
            motorRunning = false;
            Serial.println("Motor stopped.");
        }
    }

    delay(1000); // Update every second
}

// ===== Function to Read Temperature from TMP36 =====
float readTemperature() {
    int adcValue = analogRead(TEMP_SENSOR_PIN);
    float voltage = adcValue * referenceVoltage / adcResolution;
    float temperatureC = (voltage - tempOffset) * 100.0; // TMP36 scaling
    return temperatureC;
}

// ===== Function to Spin the Motor =====
void spinMotor() {
    if (!motorRunning) {
        // Set motor direction (e.g., forward)
        digitalWrite(MOTOR_IN1_PIN, HIGH);
        digitalWrite(MOTOR_IN2_PIN, LOW);
        analogWrite(MOTOR_ENABLE_PIN, 255); // Full speed
        motorRunning = true;
        motorStartTime = millis();
        Serial.println("Motor spinning...");
    }
    else {
        Serial.println("Motor is already running.");
    }
}

// ===== Function to Stop the Motor =====
void stopMotor() {
    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, LOW);
    analogWrite(MOTOR_ENABLE_PIN, 0);
}
