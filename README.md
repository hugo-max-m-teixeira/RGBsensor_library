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
  <strong>
  Obervação: a ordem dos pinos informados na função "setRGBpins()" deve ser: primeiro o pino do LED vermelho, depois do verde e logo em seguida do azul. Além disso, lembre-se de que os pinos informados devem ter a função de saída digital.
  </strong>
  
  Chegou a vez do LDR:
  
  ```cpp
    sensor.setLDRpin(A0); // Pino onde está conectado o LDR, resistor dependente de luz
  ```
  Observação: Lembrando que o pino de leitura do LDR deve estar conectado a uma porta de leitura analógica.

  Todas essas funções normalmente são colocadas na função de configuração do Arduino (void setup). Assim, o código que estamos a construir ficaria mais ou menos assim (com os pinos fictícios que colocamos nas funçoes):
  ### Pseudo código
```ino
  #include <RGBsensor.h>	// Include RGBsensor library

  RGBsensor sensor;	      // Creating RGBsensor object (this will be our sensor's name)

  void setup (){

    sensor.setRGBpins(4, 5, 6); // LED RGB pins (red, green, blue)
    sensor.setLDRpins(A0);      // LDR pin (analog input)

  }
```

# Métodos
 ## Calibração:
  ```cpp
    sensor.setBlank();  // Reliza a calibração do sensor, adquirindo os valores de refletência na cor branca
  ```
  Observação: Quando esse método é chamado, o sensor deve estar posicionado sobre a cor branca. Os valores adquiridos nas leituras realizadas serão usados como referência para a comparação das demais cores.
  "sensor" - nome do objeto (nome dado ao sensor no programa)

<br>

  ```cpp
    sensor.setBlackPercentage(uint8_t value);
  ```
  Seta (configura) o valor de corte para considerar a cor identificada como preto. O valor deve ser informado em porcentagem.
  Obs.: O valor de corte padrão para a cor preta é 70%.
<br>

## Realizar leitura e retornar cor:
  ```cpp
    char color = sensor.getColor();
  ```
  Observação: Os valores de retorno dessa função (a variável acima, color, podem assumir os seguintes valores):
  <ul>
    <li>'R' - cor vermelha</li>
    <li>'G' - cor verde</li>
    <li>'B' - cor azul</li>
  </ul>
  O valor retornado será a cor (dentre as listadas acima) que apresentou maior refletância durante a leitura e comparação.


## Realizar leitura (somente):
  ```cpp
    sensor.readColor();
  ```
  Quando chamado, esse método realiza o acendimento de cada uma das cores do LED RGB e captura a reflectância, por meio do LDR, de cada uma das cores do LED.

## Retornar a cor por ordem de reflectância:
```cpp
  char color;

  color = sensor.getReflectanceOrder(uint8_t index);
```
  Retorna a letra correspondente à cor que teve reflectância correspondente ao index informado.
  Os valores possíveis para index são:
  <ul>
    <li>index = 0 -> cor que teve <strong>maior</strong> reflectância;</li>
    <li>index = 1 -> cor que teve reflectância <strong>intermediária</strong>;</li>
    <li>index = 2 -> cor que teve <strong>menor</strong> reflectância</li>
  </ul>

  Os Valores de retorno possíveis para essa função seguem o mesmo padrão da <a href="https://github.com/hugo-max-m-teixeira/RGBsensor_library#realizar-leitura-e-retornar-cor">função getColor()</a>, ou seja:
   <ul>
    <li>'R' - cor vermelha</li>
    <li>'G' - cor verde</li>
    <li>'B' - cor azul</li>
  </ul>

<br>

## Porcentagem de refletância:
```cpp
  int reflectance;

  reflectance = sensor.getPerCent(char color);
  //or
  reflectance = sensor.getPerCent(uint8_t index);
```
  Retorna a reflectância da cor informada, em porcentagem (0 - 100%), da última leitura realizada pelo sensor. Os valores possíveis para color seguem o mesmo padrão da <a href="https://github.com/hugo-max-m-teixeira/RGBsensor_library#realizar-leitura-e-retornar-cor">função getColor()</a>, ou seja:
   <ul>
    <li>'R' - cor vermelha</li>
    <li>'G' - cor verde</li>
    <li>'B' - cor azul</li>
  </ul>

  Os valores possíveis para index são:
   <ul>
    <li>0 - cor vermelha</li>
    <li>1 - cor verde</li>
    <li>2 - cor azul</li>
  </ul>

  Lembrando que, antes de ser adquirida a reflectância de qualquer cor, <strong>ao menos uma leitura deve ser realizada </strong>, com a <a hfef="https://github.com/hugo-max-m-teixeira/RGBsensor_library#realizar-leitura-somente">função readColor()</a>;

<br>

## Checa se a cor é preta
 ```cpp
  bool result;

  result = sensor.isBlack();
```
 Lembrando que, antes de verificar se a cor é preta, <strong>ao menos uma leitura deve ser realizada </strong>, com a <a hfef="https://github.com/hugo-max-m-teixeira/RGBsensor_library#realizar-leitura-somente">função readColor()</a>;

<br>

## Para uma melhor compreensão, quel tal ver um <a href="https://github.com/hugo-max-m-teixeira/RGBsensor_library/blob/master/examples/simple_color_identification/simple_color_identification.ino">exemplo de código</a>

<br><br>

Circuito dos LEDs para a montagem do sensor:
<br>
<centered>
<img src="https://lh3.googleusercontent.com/pw/AM-JKLX6JTpDHBP50dBqnG3bwnBzCkpaOF5wh4Fc9OgkYYe0CTNVMIpVxwUcUVzozkrkBF8ycurTQ2PYptZy5M6mjgJZi2IMfWKmt_VVAEeanrl9aEFmdDf4HogreXLsuzyqFEEEBVEHVj7i2zUQJyHUIlI=w523-h479-no?authuser=0" alt="LED RGB circuit">
</centered>

<br>

Circuito do LDR para montagem do sensor:
<br>
<centered>
<img src="https://lh3.googleusercontent.com/pw/AM-JKLVooRc7L8dB-TpldFEa9NZW3qI8LndtBVRrxjRPF1sbqRHDUFajTqYL70ggdA9lcXr9AL6yvvAdMhLznOt9NNCJVk-8OQvFuIDkToBRSDWkpjIf7yZDGHMFhxj4lHYPR-bTEU_-e_D2nyoZTQWTCb8=w423-h417-no?authuser=0" alt="LRD circuit">
</centered>

Circuito completo com LED RGB:
<br>
<img class="teste" src="https://lh3.googleusercontent.com/pw/AL9nZEUd7jZHjX4IssW9GO13bndIgVJQnCKGh9MjIqKscCKwm8PItvW2EFku8Rm-Ffj9u3meC_Q9B88BsbmVMZfePhURo5dxcFb_PLuik5herZjohwvFJejyPRc9D3O7FU5Eq6VXrOthFyIBIFk5wa8N-DQs=w774-h531-no?authuser=0" alt="Complete sensor circuit">
