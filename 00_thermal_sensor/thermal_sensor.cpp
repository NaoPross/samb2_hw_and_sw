/* Naoki Pross 25.11.2015
 *
 * This program controls an RGB LED,
 * depending on the temperature the LED is red if the temperature
 * is greater than 0°C, blue if less than 0°C and green if the temperature
 * is exactly 0°C.
 *
 * In order to see small temperature variations (Ex. hold the sensor
 * between your fingers) a button can be connected to the pin no
 * TEMP_MAGNIFY_BTN. By pressing the button the program will store a
 * the temperature in that moment, then the led will turn red if the
 * temperature is greater than the stored one, blue if it is less and
 * green if equal.
 *
 * NOTE:
 * `thermal_sensor.h` generated by EclipseIDE
 * and it is supposed to contain definitions
 * and function definitions but for the sake
 * of simplicity I'll do everything in one
 * single document.
 */
#include "thermal_sensor.h"

#define LED_R 4
#define LED_G 3
#define LED_B 2

#define TEMP_SENSOR A0
#define TEMP_MAGNIFY_BTN 5
#define TEMP_MAGNIFY_COEFFICENT 25			// 25% which is 4 times more sensible

float ref_deg_temp;

void set_led_rgb(float);					// rgb control
void serial_log();							// log to serial monitor

void setup()
{
	Serial.begin(9600);

	pinMode(LED_R, OUTPUT);
	pinMode(LED_G, OUTPUT);
	pinMode(LED_B, OUTPUT);

	pinMode(TEMP_SENSOR, INPUT);
	pinMode(TEMP_MAGNIFY_BTN, INPUT);
}

void loop()
{
	int raw_temp = analogRead(TEMP_SENSOR);
	int magnify = digitalRead(TEMP_MAGNIFY_BTN);

	float deg_temp = 37.3 / 1024 * raw_temp;

	if (magnify == HIGH) {					// magnifying mode button ON/OFF
		ref_deg_temp = deg_temp; 			// setting up the reference value
		set_led_rgb(ref_deg_temp);
	}
	else {
		ref_deg_temp = 0;					// reset the temperature value
		set_led_rgb(0);
	}

	serial_log();
	delay(250);
}

void set_led_rgb(float ref_v)
{
	int red, green, blue;

	// if the temperature is positive
	if (deg_temp > ref_v) {
		blue = green = 0;
		red = (deg_temp - ref) / scale * 255;
	}
	// it the temperature is negative
	else if (deg_temp < ref_v) {
		red = green = 0;
		blue = (deg_temp + ref_v) / scale * 255;
	}
	// it the temperature is exactly 0°C
	else {
		red = blue = 0;
		green = 150;
	}

	/* NOTE
	 * the value has to be the subtracted to 255
	 * to get the opposite value because of how
	 * the circuit is made.
	 */
	analogWrite(LED_R, 255 - red);
	analogWrite(LED_G, 255 - green);
	analogWrite(LED_B, 255 - blue);
}

void serial_log()
{
	Serial.print("raw<");
	Serial.print(raw_temp);
	Serial.print("> deg<");
	Serial.print(deg_temp);
	Serial.print("> magnify<");
	Serial.print(magnifying);
	Serial.print("> ref<");
	Serial.print(ref_deg_temp);
	Serial.print("> diff<");
	Serial.print(deg_temp - ref_deg_temp);
	Serial.println(">.");
}
