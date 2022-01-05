/*
	RGBsensor library
	Simple example code using RGBsensor library for color identification
	by: Hugo Max M. Teixeira
	GitHub repository: https://github.com/hugo-max-m-teixeira/RGBsensor_library
	Date: 01/2022
*/


#include <RGBsensor.h>	// Include RGBsensor library

RGBsensor sensor;	// Creating RGBsensor object named sensor (this will be our sensor's name)

void setup () {

	Serial.begin(9600);	// Activating Serial monitor, for printing information on monitor
	
	sensor.setLDRpin(A7);			// A entrada de sinal do sensor LDR está conectada na porta A7 (obrigatoriamente essa entrada de sinal deve ser em uma porta analógica)
	sensor.setRGBpins(A0, A5, A3);	// Os pinos do nosso LED RGB (red - pino A0, green - pino A5, blue - pino A3. A ordem dos pinos informada deve respectivamente para os LEDs vermelho, verde e azul. Os pinos informados deve funcionar como saída digital.)
	sensor.setPins();				// Configura os pinos do LDR e do LED RGB.

	sensor.setBlank();				// Realiza a leitura da cor branca, para servir como referência de comparação para as demais cores.
	delay(5000);						// Aguarda 5 segundos
}

void loop () {

	// Essas serão as letras que representarão as cores lidas pelo sensor:
	
	// R = red = vermelho
	// G = green = verde
	// B = blue = azul
	// W = while = branco
	
	char cor;	// Variável que armazenará a letra que representa a cor lida pelo sensor
	
	cor  = sensor.getColor();	// cor recebe a cor lida pelo sensor (na verdade a letra que representa a cor)

	Serial.print("A cor lida pelo sensor é: ");
	Serial.println(cor);
	
	Serial.print("\n");			// Pula uma linha no monitor serial
	
	delay(1000);	// Aguarda 1 segundo
}
