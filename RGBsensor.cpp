/*
	RGBsensor library
	file .cpp
	by: Hugo Max M. Teixeira
	GitHub repository: https://github.com/hugo-max-m-teixeira/RGBsensor_library
	Date: 01/2022
*/

#include <RGBsensor.h>

void RGBsensor::setLDRpin(uint8_t pin){	pin_ldr = pin;	}

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

char RGBsensor::getRefletanceOrder(uint8_t pos){	return refletance_order[pos];	}

int RGBsensor::getR(){	return color_value[0];	}

int RGBsensor::getG(){	return color_value[1];	}

int RGBsensor::getB(){	return color_value[2];	}

int RGBsensor::getColor(char color){	return color_value[charToIndex(color)];	}

int RGBsensor::getColor(int index){	return color_value[index];	}

int RGBsensor::getBlank(char color){	return blank_value[charToIndex(color)];	}

int RGBsensor::getBlank(int index){	return blank_value[index];	}

void RGBsensor::setBlank(char color, int value){	 blank_value[charToIndex(color)] = value;	}

void RGBsensor::setBlank(int index, int value){	 blank_value[index] = value;	}

int RGBsensor::getPerCent(char color){	return percent_value[charToIndex(color)];	}

int RGBsensor::getPerCent(int index){	return percent_value[index];	}

void RGBsensor::setCutoff(uint16_t val){	cutoff_percent_value = val;	}

void RGBsensor::setMinColorVariatin(uint16_t val){	min_color_variation = val;	}

void RGBsensor::setHighTime(uint16_t time){	high_time = time;	}

void RGBsensor::setLowTime(uint16_t time){	low_time = time;	}

void RGBsensor::setBlackPercentage(float percentage) {	black_percentage = 1.0 - (percentage/100.0);	}

void RGBsensor::commonAnode (){	this->common_anode = true;	}

void RGBsensor::turn(char color, bool state){	turn(charToIndex(color), state);	}

void RGBsensor::turn(int color_num, bool state){
	if (common_anode) state = !state;
	digitalWrite(pin_led[color_num], state);
}

void RGBsensor::setFirstReadingMultiplier(float multiplier){	first_reading_multiplier = multiplier;	}

bool RGBsensor::isBlack(){
	bool black = true;
	for(int i=0; i<3; i++){
		if(color_value[i] > (blank_value[i] * black_percentage)){ // Try to false the variable, verifaca se algum pasa do valor esperado
			/*Serial.println("\t\tValor da cor: " + String(color_value[i]));
			Serial.println("\t\tValor da comparação: " + String((blank_value[i] * black_percentage)));
			Serial.println("\t\tValor percentage: " + String(black_percentage));*/
			black = false; 
		}
	}
	return black;
}

void RGBsensor::setBlank(){
	turn(2, 1);
	delay(500);
	turn(2, 0);
	for(int i=0; i<3; i++){	// For each LED
		turn(i, 1);												// turn on the led "i"
		delay(high_time);										// Wait a little moment (*1.8)
		blank_value[i] = analogRead(pin_ldr);					// Read the light(refletance) value
		turn(i, 0);												// Turn off the led "i"
		delay(low_time);										// Wait a moment while the LED stops emitting light	
	}
	last_lecture = millis();
	total_lecture_time = high_time + low_time;
}

void RGBsensor::readColor(){
	unsigned int delay_high = computeDelay(millis(), last_lecture, high_time);
	unsigned int delay_low = computeDelay(millis(), last_lecture, low_time);
	turn(0, 1);												// turn on the led "i"
	delay(delay_high);										// Wait a little moment
	color_value[0] = analogRead(pin_ldr);					// Read the light(refletance) value
	turn(0, 0);												// turn on the led "i"
	for(int i=1; i<3; i++){	// For each LED
		turn(i, 1);												// turn on the led "i"
		delay(high_time);										// Wait a little moment
		color_value[i] = analogRead(pin_ldr);					// Read the light(refletance) value
		turn(i, 0);												// Turn off the led "i"
		delay(low_time);										// Wait a moment while the LED stops emitting light
		
	}
	compareValues();	
	last_lecture = millis();
}

unsigned int RGBsensor::computeDelay(unsigned long actual_time, unsigned long last_time, unsigned int default_delay){
	unsigned int result;
	unsigned int delta_time = actual_time - last_time;
	if(delta_time < (total_lecture_time*2)){
		result = default_delay;
	} else {
		result = default_delay * (delta_time/3);
		if(result > (total_lecture_time*first_reading_multiplier)){
			result = default_delay*first_reading_multiplier;
		}
	}
	return result;
}

void RGBsensor::compareValues() {	
	for(int i=0; i<3; i++){	// Compare the values betwen the colors (red, green and blue)
		compared_value[i] = blank_value[i] - color_value[i];
	}
	
	computePerCent();
	
	int lower = percent_value[0];
	for(int i:percent_value){	// Look for the lower value of the array
		if(i < lower) lower = i;
	}
	
	int higher = percent_value[0];
	for(int i:percent_value){	// Look for the higher value of the array
		if(i > higher)	higher = i;
	}
	
	int second_lower = percent_value[0];
	for(int i:percent_value){	// Middle value
		if((i != lower) && (i != higher))	second_lower = i;
	}
	
	bool white = true;	// Checks if the color is white (difference too small betwen blank_value and color_value)
	for(int i:percent_value){
		if((abs(i) > cutoff_percent_value) && ((higher - lower) >= min_color_variation)) white = false;
	}
	
	if(white){
		color = 'W';
		for(int i=0; i<3; i++) refletance_order[i] = 'W';
	} else {		
		color = numberPerCentToColor(higher);

		refletance_order[0] = color;
		refletance_order[1] = numberPerCentToColor(second_lower);
		refletance_order[2] = numberPerCentToColor(lower);
	}
}

void RGBsensor::computePerCent(){
	for(int i=0; i<3; i++){
		percent_value[i] = (double(color_value[i]) / double(blank_value[i]) * 100.0);
	}
}

char RGBsensor::numberPerCentToColor (int value){
	char result;
	if(value == percent_value[0]) result='R';
	else if(value == percent_value[1]) result='G';
	else if(value == percent_value[2]) result='B';
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

//////////////////////////////// manyRGBsensors class ////////////////////////////////
/*
void manyRGBsensors::addSensor(RGBsensor *new_sensor){
	amount++;
	sensors = realloc(sensors, amount*sizeof(RGBsensor));
	sensors[amount - 1] = new_sensor;
}*/
