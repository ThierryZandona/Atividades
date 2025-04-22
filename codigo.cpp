#include <Arduino.h>

int pinLeds [3]{1, 2, 3};

String palavra = "";

void setup()
{

  Serial.begin(9600);
  Serial.println("selecione o led:");
  for (int a = 0; a < 3; a++)
    pinMode(pinLeds[a], OUTPUT);
}

void loop()
{

  while (Serial.available())
  {
    char caractere = Serial.read();

    if (caractere == '\n')

      Serial.println(palavra);
      for (int b = 0; b < 3; b++){}
    if (palavra)
    {
      Serial.printf(pinLeds[b]);
    }
  }
}
