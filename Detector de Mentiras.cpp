#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#define led1 2
#define led2 18
#define botao 23
// *** instacia de Objetos ***
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  randomSeed(analogRead(36));
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(botao, INPUT_PULLUP);

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("DETECTOR LIGAR");
  delay(3000);
  lcd.print("       ");
  lcd.print("PRESSIONE O BOTAO");
}

void loop()
{
  if (digitalRead(botao) == LOW)

  {
    lcd.clear();
    lcd.print("ANALISANDO...");
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);

    for (int i = 0; i < 5; i++)
    {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      delay(500);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      delay(500);
    }
    int resultado = random(2);
    if (resultado == 1)
    {
      lcd.setCursor(0,0);
      lcd.print("VERDADEIRO!");
      digitalWrite(led2, HIGH);
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print("MENTIROSO!");
      digitalWrite(led1, HIGH);
    }
    
    delay(3000);
    digitalWrite(led2, LOW);
    digitalWrite(led1, LOW);
    lcd.setCursor(0,0);
    lcd.print("PRESSIONE O BOTAO");
  }
}