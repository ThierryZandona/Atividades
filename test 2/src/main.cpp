#include <Wire.h>
#include <VL53L0X.h>
#include <LiquidCrystal_I2C.h>

#define XSHUT_Sensor1 5
#define XSHUT_Sensor2 22
#define ledVerde 1
#define ledVermelho 2
#define Sensor1_endereco 42
#define Sensor2_endereco 43

VL53L0X Sensor1;
VL53L0X Sensor2;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int distanciaMin = 0;   
const int distanciaMax = 25;  

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  pinMode(XSHUT_Sensor1, OUTPUT);
  pinMode(XSHUT_Sensor2, OUTPUT);

  digitalWrite(XSHUT_Sensor1, LOW);
  digitalWrite(XSHUT_Sensor2, LOW);
  delay(10);

  digitalWrite(XSHUT_Sensor1, HIGH);
  delay(10);
  Sensor1.init();
  Sensor1.setAddress(Sensor1_endereco);

  digitalWrite(XSHUT_Sensor2, HIGH);
  delay(10);
  Sensor2.init();
  Sensor2.setAddress(Sensor2_endereco);

  Sensor1.setTimeout(500);
  Sensor2.setTimeout(500);
  Sensor1.startContinuous();
  Sensor2.startContinuous();

  lcd.init();
  lcd.backlight();
}

void loop() {
  uint16_t measure1 = Sensor1.readRangeContinuousMillimeters();
  uint16_t measure2 = Sensor2.readRangeContinuousMillimeters();

  float distancia1 = measure1 / 10.0;
  float distancia2 = measure2 / 10.0;

  bool vaga1Ocupada = (distancia1 > distanciaMin && distancia1 <= distanciaMax);
  bool vaga2Ocupada = (distancia2 > distanciaMin && distancia2 <= distanciaMax);

  int totalLivres = 0;
  if (!vaga1Ocupada) totalLivres++;
  if (!vaga2Ocupada) totalLivres++;

  int totalOcupadas = 2 - totalLivres;

  // LEDs: Se todas livres → verde, senão → vermelho
  if (totalLivres == 2) 
  {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
  } 
  else
   {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
  }

  Serial.print("Distancia S1: ");
  Serial.print(distancia1);
  Serial.print(" cm | S2: ");
  Serial.print(distancia2);
  Serial.println(" cm");

  Serial.print("Vagas Livres: ");
  Serial.print(totalLivres);
  Serial.print(" | Ocupadas: ");
  Serial.println(totalOcupadas);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Livres: ");
  lcd.print(totalLivres);
  lcd.print(" Ocup: ");
  lcd.print(totalOcupadas);

  lcd.setCursor(0, 1);
  lcd.print("S1:");
  lcd.print(distancia1);
  lcd.print(" S2:");
  lcd.print(distancia2);

  delay(500);
}
