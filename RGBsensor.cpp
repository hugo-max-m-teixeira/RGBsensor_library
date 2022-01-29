/*
	RGBsensor library
	file .cpp
	by: Hugo Max M. Teixeira
	GitHub repository: https://github.com/hugo-max-m-teixeira/RGBsensor_library
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

int RGBsensor::getR(){
	return color_value[0];
}

int RGBsensor::getG(){
	return color_value[1];
}

int RGBsensor::getB(){
	return color_value[2];
}

int RGBsensor::getColor(char color){
	return color_value[charToIndex(color)];
}

int RGBsensor::getBlank(char color){
	return blank_value[charToIndex(color)];
}

int RGBsensor::getPerCent(char color){
	int max_refletance = blank_value[charToIndex(color)];	//Gets the blank value
	return (float(getColor(color)) / float(max_refletance)) * 100.0;	// Compare
}

void RGBsensor::setCutoff(uint16_t val){
	cutoff_value = val;
}

void RGBsensor::setHighTime(uint16_t time){	//Time that the LED keeps ON
	high_time = time;
}

void RGBsensor::setLowTime(uint16_t time){	//Time that the LED keeps OFF
	low_time = time;
}

void RGBsensor::commonAnode (){
	this->common_anode = true;
}

void RGBsensor::setBlank(){
	for(int i=0; i<3; i++){	// For each LED
		digitalWrite(pin_led[i], (common_anode ? LOW : HIGH));	// turn on the led "i"
		delay(high_time);										// Wait a little moment (*1.8)
		blank_value[i] = analogRead(pin_ldr);					// Read the light(refletance) value
		digitalWrite(pin_led[i], (common_anode ? HIGH : LOW));	// Turn off the led "i"
		delay(low_time);										// Wait a moment while the LED stops emitting light
	}
}

void RGBsensor::readColor(){
	for(int i=0; i<3; i++){	// For each LED
		digitalWrite(pin_led[i], (common_anode ? LOW : HIGH));	// turn on the led "i"
		delay(high_time);										// Wait a little moment
		color_value[i] = analogRead(pin_ldr);					// Read the light(refletance) value
		digitalWrite(pin_led[i], (common_anode ? HIGH : LOW));	// Turn off the led "i"
		delay(low_time);										// Wait a moment while the LED stops emitting light
	}
	compareValues();	
}

void RGBsensor::compareValues() {	
	for(int i=0; i<3; i++){	// Compare the values betwen the colors (red, green and blue)
		compared_value[i] = blank_value[i] - color_value[i];
	}
	
	bool white = true;	// Checks if the color is white (difference too small betwen blank_value and color_value)
	for(int i:compared_value){
		if(abs(i) > cutoff_value) white = false;
	}
	
	if(white){
		color = 'W';
		for(int i=0; i<3; i++) refletance_order[i] = 'W';
	} else {
		int lower = compared_value[0];
		for(int i:compared_value){	// Look for the lower value of the array
			if(i < lower) lower = i;
		}
		
		int higher = compared_value[0];
		for(int i:compared_value){	// Look for the higher value of the vector
			if(i > higher)	higher = i;
		}
		
		int second_lower = compared_value[0];
		for(int i:compared_value){	// Middle value
			if((i != lower) && (i != higher))	second_lower = i;
		}
		
		color = numberToColor(lower);

		refletance_order[0] = color;
		refletance_order[1] = numberToColor(second_lower);
		refletance_order[2] = numberToColor(higher);
	}
}

char RGBsensor::numberToColor (int value){
	char result;
	if(value == compared_value[0]) result='R';
	else if(value == compared_value[1]) result='G';
	else if(value == compared_value[2]) result='B';
	return result;
}

uint8_t RGBsensor::charToIndex(char color){
	int value;
	if ((color == 'R') || (color == 'r'))	value = 0;
	else if ((color == 'G') || (color == 'g'))	value = 1;
	else if ((color == 'B') || (color == 'b'))	value = 2;
	else	value = 0;
	return value;
}
