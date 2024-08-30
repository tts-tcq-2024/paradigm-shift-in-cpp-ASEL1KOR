#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// Thresholds for battery health parameters
const float MIN_TEMPERATURE = 0.0;
const float MAX_TEMPERATURE = 45.0;
const float MIN_SOC = 20.0;
const float MAX_SOC = 80.0;
const float MAX_CHARGE_RATE = 0.8;

// Enum to indicate the status of the measure
enum MeasureStatus {
    OK,
    OUT_OF_RANGE
};

// Pure function to get the status of a parameter
MeasureStatus getMeasureStatus(float value, float min, float max) {
    return (value < min || value > max) ? OUT_OF_RANGE : OK;
}

// Function to check a single battery parameter and return a message if out of range
bool checkParameter(float value, float min, float max, const string& paramName, string& breachMessage) {
    if (getMeasureStatus(value, min, max) == OUT_OF_RANGE) {
        breachMessage += paramName + " ";
        return false;
    }
    return true;
}

// Function to check all battery parameters and return whether it's OK
bool batteryIsOk(float temperature, float soc, float chargeRate, string& breachMessage) {
    breachMessage = "";
    bool isOk = true;

    isOk &= checkParameter(temperature, MIN_TEMPERATURE, MAX_TEMPERATURE, "Temperature", breachMessage);
    isOk &= checkParameter(soc, MIN_SOC, MAX_SOC, "State of Charge", breachMessage);
    isOk &= checkParameter(chargeRate, 0, MAX_CHARGE_RATE, "Charge Rate", breachMessage);

    if (!isOk) {
        breachMessage = "Battery is NOT OK due to " + breachMessage + "being out of range.\n";
    } else {
        breachMessage = "Battery is OK.\n";
    }

    return isOk;
}

// Function to test various battery conditions
void testBatteryIsOk() {
    string message;

    // Normal condition
    assert(batteryIsOk(25, 70, 0.7, message) == true);

    // Abnormal conditions - out of range
    assert(batteryIsOk(50, 70, 0.7, message) == false);  // Temperature too high
    assert(batteryIsOk(-5, 70, 0.7, message) == false);  // Temperature too low
    assert(batteryIsOk(25, 90, 0.7, message) == false);  // SOC too high
    assert(batteryIsOk(25, 10, 0.7, message) == false);  // SOC too low
    assert(batteryIsOk(25, 70, 1.0, message) == false);  // Charge rate too high

    // Boundary conditions
    assert(batteryIsOk(0, 20, 0, message) == true);  // All parameters at the lower boundary
    assert(batteryIsOk(45, 80, 0.8, message) == true);  // All parameters at the upper boundary
}

int main() {
    testBatteryIsOk();  // Running the tests
    
    string message;
    cout << batteryIsOk(25, 70, 0.7, message) << ": " << message;  // Normal condition
    cout << batteryIsOk(50, 70, 0.7, message) << ": " << message;  // Temperature too high
    cout << batteryIsOk(25, 90, 0.9, message) << ": " << message;  // SOC too high, charge rate too high
    
    return 0;
}
