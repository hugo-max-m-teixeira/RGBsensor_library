/*
	RGBsensor library
	file .h
	by: Hugo Max M. Teixeira
	GitHub repository: https://github.com/hugo-max-m-teixeira/RGBsensor_library
	Date: 01/2022
*/

#ifndef RGBsensor_h
#define RGBsensor_h

#include <Arduino.h>	// Arduino Library

class RGBsensor {
public:
	// Pins...
	void setLDRpin(uint8_t pin);
	void setRGBpins(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b);
	void setPins();	// pinMode and etc.
	
	
	//Basic methods
	void setBlank();	// Set the blank color
	char getColor();	// Read and return the strongest actual color ('R', 'G' or 'B')
	
	// Other methods
	void readColor();			// Read values of actual color
	char getRefletanceOrder(uint8_t pos);	// Return the refletance order (pos=0 => higher refletance, pos=2 => lower refletance))
	int getR();	//Returns the refletance of the red color
	int getG();	//Returns the refletance of the green color
	int getB();	//Returns the refletance of the blue color
	int getColor(char color);
	int getBlank(char color);
	void setCutoff(uint16_t val);	//Sets the cutoff value, for white color detect
	int getPerCent(char color);
	void setHighTime(uint16_t time);	//Time that the LED keeps ON
	void setLowTime(uint16_t time);		//Time that the LED keeps OFF
	void commonAnode();	// Common anode LED (don't call this method if your LED is common cathod)

private:
	uint8_t pin_ldr;	//LDR pin (light sensor)
	uint8_t pin_led[3];	//LEDs pins (pin_led[0] = red, pin_led[1] = green, pin_led[2] = blue)
	int blank_value[3], color_value[3], compared_value[3], cutoff_value = 22;	// Light values
	char color = "";	// Color => 'R' =  red,     'G' = green,     'B' = blue
	char refletance_order[3] = {""};	// Descending order of refletance
	void compareValues();		// Compare the values betwen white color and actual color
	char numberToColor (int value);
	uint8_t charToIndex(char color);
	uint16_t high_time = 20, low_time = 20;	//Default times HIGH and LOW
	bool common_anode = false;
};

#endif
