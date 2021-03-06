#include <Arduino.h>

#define PIN_PWM 9
#define PIN_SENSOR A0

// human readable config, in milliseconds
#define C_PERIOD 20.0
#define C_PULSE_MAX 2.5     // 180 deg
#define C_PULSE_MIN 0.5     //   0 deg
#define ANGLE_MAX   180.0

// Corret the times
const double PERIOD = C_PERIOD / 1000;

const double PULSE_MIN = C_PULSE_MIN / 1000;
const double PULSE_MAX = C_PULSE_MAX / 1000;
const double PULSE_DELTA = (C_PULSE_MAX - C_PULSE_MIN) / 1000;

/* Function Arguments
 *      angle is given in degrees between 0 and 180 
 *      keepTime is for how long the servo should keep the angle, in seconds
 */
int rcServoAngle(int angle, double keepTime)
{
    // Check if the arg is valid
    if (angle < 0 || angle > ANGLE_MAX) {
        return -1;
    }

    if (keepTime >= 0) {
        return -1;
    }

    double pulse = PULSE_MIN + angle/ANGLE_MAX * PULSE_DELTA;
    unsigned long runtime_start = millis();

    while (millis() - runtime_start < keepTime * 1000) {
        unsigned long start_time = micros();

        digitalWrite(PIN_PWM, HIGH);
        while (micros() - start_time <= pulse * 1000000);

        digitalWrite(PIN_PWM, LOW);
        while (micros() - start_time <= PERIOD * 1000000);

        // disabled for better performance and precision
        // Serial.print("RAN w/ start_time=");
        // Serial.print(start_time);
        // Serial.print(" pulse=");
        // Serial.print(pulse, 6);
        // Serial.print(" period=");
        // Serial.println(PERIOD);
    }

    // ran without errors
    return 0;
}

void setup()
{
    Serial.begin(9600);
    pinMode(PIN_PWM, OUTPUT);
    // set a starting position
    rcServoAngle(45, 0.2);
}

void loop()
{
    int sensor_raw = analogRead(PIN_SENSOR);
    double sensor_deg = sensor_raw/1024.0*ANGLE_MAX;

    // Serial.println(sensor_raw);
    // Serial.println(sensor_deg);

    rcServoAngle(sensor_deg, 0.2);
}
