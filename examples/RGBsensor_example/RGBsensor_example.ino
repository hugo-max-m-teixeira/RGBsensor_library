#include <RGBsensor.h>	// Include RGBsensor library

RGBsensor sensor;	// Creating RGBsensor object named sensor (this will be our sensor's name)

void setup () {
	Serial.begin(9600);	// Activating Serial monitor, for printing information on monitor
	
	sensor.setLDRpin(A7);			// A entrada de sinal do sensor LDR está conectada na porta A7 (obrigatoriamente essa entrada de sinal deve ser em uma porta analógica)
	sensor.setRGBpins(A0, A5, A3);	// Os pinos do nosso LED RGB (red - pino A0, green - pino A5, blue - pino A3. A ordem dos pinos informada deve respectivamente para os LEDs vermelho, verde e azul. Os pinos informados deve funcionar como saída digital.)
	sensor.setPins();				// Configura os pinos do LDR e do LED RGB.

	delay(200);						// Aguarda 200 ms
	sensor.setBlank();				// Realiza a leitura da cor branca, para servir como referência de comparação para as demais cores.
}

void loop () {

	Serial.print("\nPress ENTER to read the sensor\n\n");
  
	char answer = '0';
	
	while(answer == '0'){
		if(Serial.available() > 0){
			answer = Serial.read();
		}
	}


	sensor.readColor();

	Serial.print("Main color: ");
	Serial.println(sensor.getRefletanceOrder(0));

	Serial.print("Second main color: ");
	Serial.println(sensor.getRefletanceOrder(1));
	
	Serial.print("Last color: ");
	Serial.println(sensor.getRefletanceOrder(2));

	Serial.print("\n");

	Serial.println("white values: ");
	Serial.print("Red: ");
	Serial.println(sensor.getBlank('R'));
	Serial.print("Green: ");
	Serial.println(sensor.getBlank('G'));
	Serial.print("Blue: ");
	Serial.println(sensor.getBlank('B'));
	
	
	Serial.println("\nColor values:");
	Serial.print("Red: ");
	Serial.println(sensor.getColor('R'));
	Serial.print("Green: ");
	Serial.println(sensor.getColor('G'));
	Serial.print("Blue: ");
	Serial.println(sensor.getColor('B'));

	Serial.print("\nVariation in percentage: \n");
	Serial.print("R : ");
	Serial.println(sensor.getPerCent('R'));
	Serial.print("G : ");
	Serial.println(sensor.getPerCent('G'));
	Serial.print("B : ");
	Serial.println(sensor.getPerCent('B'));
	

	Serial.print("\n\n\n-----------------------------");
	
}
