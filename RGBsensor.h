/*
	RGBsensor library
	file .h
	by: Hugo Max M. Teixeira
	GitHub: https://github.com/hugo-max-m-teixeira
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
	char getColor();	// Read and return the actual color ('R', 'G' or 'B')
	
	// Other methods
	char getRefletanceOrder(uint8_t pos);	// Return the refletance order (pos=0 => higher refletance, pos=2 => lower refletance))
	uint16_t getR();	//Returns the refletance of the red color
	uint16_t getG();	//Returns the refletance of the green color
	uint16_t getB();	//Returns the refletance of the blue color
	void setCutoff(uint16_t val);	//Sets the cutoff value, for white color detect
	

private:
	uint8_t pin_ldr;	//LDR pin (light sensor)
	uint8_t pin_led[3];	//LEDs pins (pin_led[0] = red, pin_led[1] = green, pin_led[2] = blue)
	uint16_t blank_value[3], color_value[3], cutoff_value = 8;	// Light values
	char color = "";	// Color => 'R' =  red,     'G' = green,     'B' = blue
	char refletance_order[3] = {""};	// Descending order of refletance
	void readColor();			// Read values of actual color
	void compareValues();		// Compare the values betwen white color and actual color
	char numberToColor (uint64_t value);	// Convert number of refletance to color

};

#endif
