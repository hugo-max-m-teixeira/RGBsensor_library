/*
	RGBsensor library
	file .cpp
	by: Hugo Max M. Teixeira
	GitHub: https://github.com/hugo-max-m-teixeira
	Date: 01/2022
*/

#include <RGBsensor.h>

void RGBsensor::setLDRpin(uint8_t pin){
	pin_ldr = pin;
}

void RGBsensor::setRGBpins(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b){
	pin_led[0] = pin_r;	pin_led[1] = pin_g;	pin_led[2] = pin_b;
}

void RGBsensor::setPins(){
	pinMode(pin_ldr, INPUT);	// LDR sensor as input
	for(int i:pin_led){
		pinMode(i, OUTPUT);		// LEDs as output
		digitalWrite(i, LOW);	// Turn the LEDs off
	}
}

char RGBsensor::getColor(){
	readColor();
	return color;
}

char RGBsensor::getRefletanceOrder(uint8_t pos){
	return refletance_order[pos];
}

uint16_t RGBsensor::getR(){
	return color_value[0];
}

uint16_t RGBsensor::getG(){
	return color_value[1];
}

uint16_t RGBsensor::getB(){
	return color_value[2];
}

void RGBsensor::setCutoff(uint16_t val){
	cutoff_value = val;
}

void RGBsensor::setBlank(){
	for(int i=0; i<3; i++){	// For each LED
		digitalWrite(pin_led[i], HIGH);	// turn on the led "i"
		delay(20);				// Wait a little moment
		blank_value[i] = analogRead(pin_ldr);	// Read the light(refletance) value
		digitalWrite(pin_led[i], LOW);	// Turn off the led "i"
	}
}

void RGBsensor::readColor(){
	for(int i=0; i<3; i++){	// For each LED
		digitalWrite(pin_led[i], HIGH);	// turn on the led "i"
		delay(20);				// Wait a little moment
		color_value[i] = analogRead(pin_ldr);	// Read the light(refletance) value
		digitalWrite(pin_led[i], LOW);	// Turn off the led "i"
	}
	compareValues();	
}

void RGBsensor::compareValues() {	
	for(int i=0; i<3; i++){	// Compare the values betwen the colors (red, green and blue)
		color_value[i] = blank_value[i] - color_value[i];
	}
	
	bool white = true;
	for(int i:color_value){
		if(i > cutoff_value) white = false;
	}
	
	if(!white){
		uint16_t lower = color_value[0];
		for(int i:color_value){	// Look for the lower value of the vector
			if(i < lower) lower = i;
		}
		
		uint16_t higher = color_value[0];
		for(int i:color_value){	// Look for the higher value of the vector
			if(i > higher)	higher = i;
		}
		
		uint16_t second_lower = color_value[0];
		for(int i:color_value){	// Middle value
			if((i != lower) && (i != higher))	second_lower = i;
		}
		
		color = numberToColor(lower);

		refletance_order[0] = color;
		refletance_order[1] = numberToColor(second_lower);
		refletance_order[2] = numberToColor(higher);
	} else {
		color = "W";
	}
}

char RGBsensor::numberToColor (uint64_t value){
	char result;
	if(value == color_value[0]) result='R';
	else if(value == color_value[1]) result='G';
	else if(value == color_value[2]) result='B';
	return result;
}

