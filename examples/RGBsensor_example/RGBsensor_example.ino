#include <RGBsensor.h>

RGBsensor sensor;

void setup () {
	Serial.begin(9600);
	
	sensor.setLDRpin(A7);
	sensor.setRGBpins(A0, A5, A3);
	sensor.setPins();
	
	sensor.setBlank();
	delay(4000);
}

void loop () {
	//sensor.readColor();
	
	/*for(uint16_t i:sensor.color_value){
		Serial.println(i);
	}*/

	
	Serial.println(sensor.getColor());
	
	delay(3000);
}
