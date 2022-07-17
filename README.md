# RGBsensor_library

Biblioteca para Arduino desenvolvida para facilitar o uso de um sensor de cor baseado em um LDR e em um LED RGB. O programa permite que o usuário:
<ul>
  <li>Calibre o sensor</li>
  <li>Identifique qual das 3 cores (dentre o vermelho, verde e azul) apresentou maior refletância</li>
  <li>Identifique a ordem de fefletância dessas 3 cores (da maior para a menor)</li>
  <li>Receber os valores de refletância em valores discretos de variações de tensão ou em porcentagem para cada uma das cores do LED RGB</>
</ul>

## Configurando o sensor:
  Lembrando que os códigos fornecidos abaixo são de exemplo, os pinos informados não precisam ser os mesmos mencionados aqui.
### Criando o objeto:
  Aqui daremos um nome ao nosso sensor, esse será o nome atribuido a ele no restante do programa.
  ```cpp
    RGBsensor sensor; // Sensor é o nome do nosso sensor
  ```
### Setando os pinos
  Agora diremos ao programa onde estão conectados os pinos dos LEDs (ou do LED) e do LDR
  ```cpp
    sensor.setRGBpins(4, 5, 6); // Pinos onde estão conectados os LEDs coloridos, respectivamente vermelho, verde e azul
  ```
  Obervação: a ordem dos pinos informados deve ser: primeiro o pino do LED vermelho, depois do verde e logo em seguida do azul. Além disso, lembre-se de que os pinos informados devem ter a função de saída digital.
  
  Chegou a vez do LDR:
  
  ```cpp
    sensor.setLDRpin(A0); // Pinos onde está conectado o LDR, resistor dependente de luz
  ```
  Observação: Lembrando que o pino de leitura do LDR deve estar conectado a uma porta de leitura analógica.

## Métodos
 ### Calibração:
  ```cpp
    sensor.setBlank();
  ```
  Observação: Quando esse método é chamado, o sensor deve estar posicionado sobre a cor branca. Os valores adquiridos nas leituras realizadas serão usados como referência para a comparação das demais cores.
  "sensor" - nome do objeto (nome dado ao sensor no programa)
  
  ### Realizar leitura e retornar cor:
  ```cpp
    char color = sensor.getColor();
  ```
  Observação: Os valores de retorno dessa função podem ser (a variável acima, color, pode assumir os seguintes valores):
  <ul>
    <li>'R' - cor vermelha</li>
    <li>'G' - cor verde</li>
    <li>'B' - cor azul</li>
  </ul>
  O valor retornado será a cor (dentre as listadas acima) que apresentou maior refletância durante a leitura e comparação.

Circuito dos LEDs para a montagem do sensor:
<br>
<centered>
<img src="https://lh3.googleusercontent.com/pw/AM-JKLX6JTpDHBP50dBqnG3bwnBzCkpaOF5wh4Fc9OgkYYe0CTNVMIpVxwUcUVzozkrkBF8ycurTQ2PYptZy5M6mjgJZi2IMfWKmt_VVAEeanrl9aEFmdDf4HogreXLsuzyqFEEEBVEHVj7i2zUQJyHUIlI=w523-h479-no?authuser=0" alt="LED RGB circuit">
</centered>

Circuito do LDR para montagem do sensor:
<br>
<centered>
<img src="https://lh3.googleusercontent.com/pw/AM-JKLVooRc7L8dB-TpldFEa9NZW3qI8LndtBVRrxjRPF1sbqRHDUFajTqYL70ggdA9lcXr9AL6yvvAdMhLznOt9NNCJVk-8OQvFuIDkToBRSDWkpjIf7yZDGHMFhxj4lHYPR-bTEU_-e_D2nyoZTQWTCb8=w423-h417-no?authuser=0" alt="LRD circuit">
</centered>

Circuito completo:
<br>
<img src="https://lh3.googleusercontent.com/pw/AM-JKLU1B5X4Q9HlVQC1_h1vLYk-IrvgI0Za2PTlnOYTnM2Vr9TckHkaDhV_UsR4X0CoW3ir-sgSIkWJjpsWpVS_ipN0YdrWzbG9MQnjNvu0QCdvt6qszhmzb1b80uQOiTBWq8sPBekfPtbdw8xpUot_xWFS=w745-h529-no?authuser=0" alt="Complete sensor circuit">
