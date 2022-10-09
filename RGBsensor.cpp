/*
	RGBsensor library
	file .cpp
	by: Hugo Max M. Teixeira
	GitHub repository: https://github.com/hugo-max-m-teixeira/RGBsensor_library
	Date: 01/2022
*/

#include <RGBsensor.h>

void RGBsensor::setLDRpin(uint8_t pin){	pin_ldr = pin;	}

uint8_t RGBsensor::getLDRpin(){	return pin_ldr;	}

void RGBsensor::setRGBpins(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b){
	pin_led[0] = pin_r;	pin_led[1] = pin_g;	pin_led[2] = pin_b;
	for(int i:pin_led){
		pinMode(i, OUTPUT);
	}
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

char RGBsensor::getReflectanceOrder(uint8_t pos){	return reflectance_order[pos];	}

int RGBsensor::getR(){	return color_value[0];	}

int RGBsensor::getG(){	return color_value[1];	}

int RGBsensor::getB(){	return color_value[2];	}

int RGBsensor::getColor(char color){	return color_value[charToIndex(color)];	}

int RGBsensor::getColor(uint8_t index){	return color_value[index];	}

int RGBsensor::getBlank(char color){	return blank_value[charToIndex(color)];	}

int RGBsensor::getBlank(uint8_t index){	return blank_value[index];	}

void RGBsensor::setBlank(char color, int value){	 blank_value[charToIndex(color)] = value;	}

void RGBsensor::setBlank(uint8_t index, int value){	 blank_value[index] = value;	}

int RGBsensor::getPerCent(char color){	return percent_value[charToIndex(color)];	}

int RGBsensor::getPerCent(uint8_t index){	return percent_value[index];	}

void RGBsensor::setCutoff(uint16_t val){	cutoff_percent_value = val;	}

void RGBsensor::setMinColorVariation(uint16_t val){	min_color_variation = val;	}

void RGBsensor::setHighTime(uint16_t time){	high_time = time;	}

void RGBsensor::setLowTime(uint16_t time){	low_time = time;	}

void RGBsensor::setBlackPercentage(uint8_t percentage) {	black_percentage = percentage/100.0;	}

void RGBsensor::setBlankValue(uint8_t color_index, uint16_t value){	blank_value[color_index] = value;	}

void RGBsensor::commonAnode (){	this->common_anode = true;	}

void RGBsensor::turn(char color, bool state){	turn(charToIndex(color), state);	}

void RGBsensor::turn(uint8_t color_num, bool state){
	if (common_anode) state = !state;
	digitalWrite(pin_led[color_num], state);
}

void RGBsensor::setFirstReadingMultiplier(float multiplier){	first_reading_multiplier = multiplier;	}

bool RGBsensor::isBlack(){
	bool black = true;
	for(int i=0; i<3; i++){
		if(color_value[i] > (blank_value[i] * black_percentage)){ // Tenta falsear a variável, verifaca se algum pasa do valor esperado
			/*Serial.println("\t\tValor da cor: " + String(color_value[i]));
			Serial.println("\t\tValor da comparação: " + String((blank_value[i] * black_percentage)));
			Serial.println("\t\tValor percentage: " + String(black_percentage));*/
			black = false; 
		}
	}
	return black;
}

void RGBsensor::setBlank(){
	turn(uint8_t(2), 1);
	delay(500);
	turn(uint8_t(2), 0);
	for(uint8_t i=0; i<3; i++){	// For each LED
		turn(i, 1);												// turn on the led "i"
		delay(high_time);										// Wait a little moment (*1.8)
		setBlankValue(i, analogRead(pin_ldr));
		//blank_value[i] = analogRead(pin_ldr);					// Read the light(refletance) value
		turn(i, 0);												// Turn off the led "i"
		delay(low_time);										// Wait a moment while the LED stops emitting light	
	}
	last_lecture = millis();
	total_lecture_time = high_time + low_time;
}

void RGBsensor::readColor(){
	unsigned int delay_high = computeDelay(millis(), last_lecture, high_time);
	unsigned int delay_low = computeDelay(millis(), last_lecture, low_time);
	turn(uint8_t(0), 1);												// turn on the led "i"
	delay(delay_high);										// Wait a little moment
	color_value[0] = analogRead(pin_ldr);					// Read the light(refletance) value
	turn(uint8_t(0), 0);												// turn on the led "i"
	for(uint8_t i=1; i<3; i++){	// For each LED
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
		for(int i=0; i<3; i++) reflectance_order[i] = 'W';
	} else {		
		color = numberPerCentToColor(higher);

		reflectance_order[0] = color;
		reflectance_order[1] = numberPerCentToColor(second_lower);
		reflectance_order[2] = numberPerCentToColor(lower);
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
	uint8_t value;
	if ((color == 'R') || (color == 'r'))	value = 0;
	else if ((color == 'G') || (color == 'g'))	value = 1;
	else if ((color == 'B') || (color == 'b'))	value = 2;
	else	value = 0;
	return value;
}

//////////////////////////////// manyRGBsensors class ////////////////////////////////

manyRGBsensors::manyRGBsensors(RGBsensor *sensors_[]){
	amount = sizeof(sensors_)/sizeof(RGBsensor);
	this->sensors = realloc(this->sensors, amount*sizeof(RGBsensor));
}

void manyRGBsensors::addSensor(RGBsensor *new_sensor){
	amount++;
	sensors = realloc(sensors, amount*sizeof(RGBsensor));
	sensors[amount - 1] = new_sensor;
	if(amount > 1){	// If there are more than one sensor...
		for(int i=1; i<amount; i++){// Each sensor
			for(int p=0; p<3; p++){	// Receives the LED pins of the first sensor
				sensors[i]->pin_led[p] = sensors[0]->pin_led[p];
			}
			sensors[i]->setHighTime(sensors[0]->high_time);
			sensors[i]->setBlackPercentage(sensors[0]->black_percentage);
		}
	}
}

void manyRGBsensors::setBlank(){
	turn(uint8_t(1), 1);
	delay(750);
	turn(uint8_t(1), 0);
	
	for(uint8_t i=0; i<3; i++){	// For each color...
		turn(i, 1);
		delay(sensors[0]->high_time);
		for(int j=0; j<amount; j++){	// For each sensor...
			sensors[j]->setBlankValue(i, analogRead(sensors[j]->getLDRpin()));
		}
		delay(sensors[0]->low_time);
		turn(i, 0);
	}
	for(int i=0; i<amount; i++){
		sensors[i]->last_lecture = millis();
		sensors[i]->total_lecture_time = (sensors[0]->high_time + sensors[0]->low_time) * 3;
	}
}

void manyRGBsensors::readColor(){
	unsigned int delay_high = sensors[0]->computeDelay(millis(), sensors[0]->last_lecture, sensors[0]->high_time);
	//unsigned int delay_low = computeDelay(millis(), sensors[0]->last_lecture, sensors[0]->low_time);
	turn(uint8_t(0), 1);												// turn on the red led (index 0)
	delay(delay_high);										// Wait a little moment
	for(int i=0; i<amount; i++){
		sensors[i]->color_value[0] = analogRead(sensors[i]->getLDRpin());	// Read the light(refletance) value for the first color (red)
	}
	turn(uint8_t(0), 0);												// turn off the red led (index 0)
	for(uint8_t i=1; i<3; i++){	// For each LED
		turn(i, 1);											// turn on the led "i"
		delay(sensors[0]->high_time);										// Wait a little moment
		for(int j=0; j<amount; j++){
			sensors[j]->color_value[i] = analogRead(sensors[j]->getLDRpin());					// Read the light(refletance) value
		}
		turn(i, 0);											// Turn off the led "i"
		delay(sensors[0]->low_time);										// Wait a moment while the LED stops emitting light
		
	}
	for(int i=0; i<amount; i++){
		sensors[i]->compareValues();
		sensors[i]->last_lecture = millis();
	}	
}

void manyRGBsensors::turn(char color, bool state){	manyRGBsensors::turn(sensors[0]->charToIndex(color), state);	}

void manyRGBsensors::turn(uint8_t color_num, bool state){
	for(int i=0; i<amount; i++){
		sensors[i]->turn(color_num, state);
	}
}
