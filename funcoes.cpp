#include <Arduino.h>
#define pinLed 21

// * PROTOTIPO DA FUNÇÃO
int calculaSegundoGrau(int, int, int);

void setup()
{
Serial.begin(9600);
int raizDaFuncao = calculaSegundoGrau(1, 6, 9);
Serial.println(raizDaFuncao);
}

void loop()
{}

int calculaSegundoGrau(int a, int b, int c)
{
int resultado;

resultado = (b * -1)+(sqrt(sq(b))-(4 * a * c))/(2 * a);



return resultado;


}
