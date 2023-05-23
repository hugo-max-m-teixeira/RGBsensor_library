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
	void setLDRpin(uint8_t pin);	//Sets the LDR pin (analog Arduino port)
	void setRGBpins(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b);	// Sets the RGB pins, respectively
	void setPins();	// pinMode and etc.
	
	//Basic methods
	void setBlank();	// Set the blank reflectance values
	void setBlack();	// Set the black reflectance values 
	char getColor();	// Read and return the strongest actual color ('R', 'G' or 'B')
	bool isBlack();
	
	// Other methods
	void readColor();			// Read values of actual color
	
	// Reflectance methods:
	char getReflectanceOrder(uint8_t pos);	// Return the refletance order (pos=0 => higher refletance, pos=2 => lower refletance))
	bool isRed();
	bool isGreen();
	bool isBlue();
	int getR();	//Returns the refletance of the red color
	int getG();	//Returns the refletance of the green color
	int getB();	//Returns the refletance of the blue color
	int getColor(char color);	//Returns the refletance of the color sensor
	int getColor(uint8_t index);
	int getBlank(char color);	// Return the blank value 
	int getBlank(uint8_t index);
	int getPerCent(char color);			// Returns the percentage of the color variance compared to the blank color value
	int getPerCent(uint8_t index);
	
	// Calibration methods
	void setBlank(char color, int value);
	void setBlank(uint8_t index, int value);
	void setBlankValue(uint8_t color_index, uint16_t value);
	void setCutoff(uint16_t val);		//Sets the cutoff value, for white color detect
	void setBlackPercentage(uint8_t percent);	// Sets the min value for color reflectance to be considered non-black
	void setMinColorVariation(uint16_t val);
	void setHighTime(uint16_t time);	//Time that the LED keeps ON
	void setLowTime(uint16_t time);		//Time that the LED keeps OFF
	void commonAnode();					// Common anode LED (don't call this method if your LED is common cathod)
	void setFirstReadingMultiplier(float multiplier);	// Sets the time multiplier for the first blank lecture
	uint8_t getLDRpin();
	void readAndStoreAt(int* variable_array);
	
	// "DigitalWrite" functions, used to turn a pin to HIGH or LOW
	void turn(char color, bool state);	//Change manually the state of the LED 
	void turn(uint8_t color_num, bool state);
	void specialDigitalWrite(void (*turn_function)(int, bool));	// If RGB pins are not connected directly to an Arduino port,
																// send the function that turns the pins (as digitalWrite does)
	bool special_turn = false;
	void (*specialTurn)(int pin, bool state);
	
	// Functions usually used only by the class
	void compareValues();		// Compare the values betwen white color and actual color
	void computePerCent();		// Compute percent variation
	char numberPerCentToColor(int value);
	uint8_t charToIndex(char color);
	unsigned int computeDelay(unsigned long actual_time, unsigned long last_time, unsigned int default_delay);
	

//private:
	unsigned long last_lecture;	// Last lecture time
	float black_percentage = 0.60;
	uint16_t min_color_variation = 7;
	unsigned int total_lecture_time;
	float first_reading_multiplier = 3;	// Multiplier for first blank lecture
	uint8_t pin_ldr;	//LDR pin (light sensor)
	uint8_t pin_led[3];	//LEDs pins (pin_led[0] = red, pin_led[1] = green, pin_led[2] = blue)
	int blank_value[3], black_value[3], color_value[3], compared_value[3], percent_value[3];	// Light values
	uint16_t cutoff_percent_value = 10;	// Cutoff value for white and color diferentiation
	char color = "";	// Color => 'R' =  red,     'G' = green,     'B' = blue
	char reflectance_order[3] = {""};	// Descending order of refletance
	uint16_t high_time = 70, low_time = 0;	//Default times HIGH and LOW
	#if defined PHOTOTRANSISTOR	// Checks if a phototransistor is being used
		high_time = 2;
		low_time = 0;
	#endif
	bool common_anode = false;
};

class manyRGBsensors{
	public:
		uint16_t amount = 0;
		RGBsensor **sensors = malloc(sizeof(RGBsensor));
		
		manyRGBsensors(RGBsensor *sensors_[]);
		manyRGBsensors(){}
		
		void addSensor(RGBsensor *new_sensor);	// Add the sensor to the array of sensors and reasize the used memory;
		void setBlank();						// Set the blank calibration values
		void readColor();						// Only read the color (don't return anything)
		char getColor();						// Read and return the most reflective identified color

		void turn(char color, bool state);	//Change manually the state of the LED 
		void turn(uint8_t color_num, bool state);
};

#endif
