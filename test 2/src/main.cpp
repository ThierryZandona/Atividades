#include <Wire.h>
#include <VL53L0X.h>
#include <LiquidCrystal_I2C.h>

// Definição de pinos
#define XSHUT_Sensor1 19
#define XSHUT_Sensor2 4
#define ledVerde 5
#define ledVermelho 19
#define ILUMINACAO_AUXILIAR 3  // Pino PWM para iluminação auxiliar
#define LDRpin 4               // Pino do sensor LDR

// Endereços I2C personalizados para os sensores VL53L0X
#define Sensor1_endereco 42
#define Sensor2_endereco 43

// Criação dos objetos dos sensores
VL53L0X Sensor1;
VL53L0X Sensor2;

// Objeto do display LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Parâmetros de distância para considerar a vaga ocupada
const int distanciaMin = 0;    // em cm
const int distanciaMax = 25;   // em cm

// Parâmetro de limiar de luminosidade
const int limiarLuminosidade = 400; // abaixo desse valor, acende luz auxiliar

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  // Configuração dos pinos
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ILUMINACAO_AUXILIAR, OUTPUT);
  pinMode(LDRpin, INPUT);

  pinMode(XSHUT_Sensor1, OUTPUT);
  pinMode(XSHUT_Sensor2, OUTPUT);

  // Desativa ambos os sensores inicialmente
  digitalWrite(XSHUT_Sensor1, LOW);
  digitalWrite(XSHUT_Sensor2, LOW);
  delay(10);

  // Inicializa Sensor1
  digitalWrite(XSHUT_Sensor1, HIGH);
  delay(10);
  Sensor1.init();
  Sensor1.setAddress(Sensor1_endereco);

  // Inicializa Sensor2
  digitalWrite(XSHUT_Sensor2, HIGH);
  delay(10);
  Sensor2.init();
  Sensor2.setAddress(Sensor2_endereco);

  // Configura o timeout e inicia leitura contínua
  Sensor1.setTimeout(500);
  Sensor2.setTimeout(500);
  Sensor1.startContinuous();
  Sensor2.startContinuous();

  // Inicializa o LCD
  lcd.init();
  lcd.backlight();
}

void loop()
{
  // Leitura das distâncias
  uint16_t measure1 = Sensor1.readRangeContinuousMillimeters();
  uint16_t measure2 = Sensor2.readRangeContinuousMillimeters();

  float distancia1 = measure1 / 10.0;  // converte para cm
  float distancia2 = measure2 / 10.0;

  // Determina se as vagas estão ocupadas
  bool vaga1Ocupada = (distancia1 > distanciaMin && distancia1 <= distanciaMax);
  bool vaga2Ocupada = (distancia2 > distanciaMin && distancia2 <= distanciaMax);

  // Contagem de vagas livres e ocupadas
  int totalLivres = 0;
  if (!vaga1Ocupada) totalLivres++;
  if (!vaga2Ocupada) totalLivres++;

  int totalOcupadas = 2 - totalLivres;

  // Controle dos LEDs: verde se livre, vermelho se ocupada
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

  // Leitura do sensor de luminosidade (LDR)
  int luzAmbiente = analogRead(LDRpin);
  Serial.print("Luz Ambiente: ");
  Serial.println(luzAmbiente);

  // Controle da iluminação auxiliar
  if (luzAmbiente < limiarLuminosidade)
  {
    int intensidade = 0;

    // Se apenas uma vaga ocupada → intensidade média
    if ((vaga1Ocupada && !vaga2Ocupada) || (!vaga1Ocupada && vaga2Ocupada)) {
      intensidade = 200;
    }
    // Se ambas ocupadas → intensidade máxima
    else if (vaga1Ocupada && vaga2Ocupada) {
      intensidade = 255;
    }
    // Se ambas livres → intensidade mínima
    else {
      intensidade = 100;
    }

    // Aplica intensidade via PWM
    analogWrite(ILUMINACAO_AUXILIAR, intensidade);
  }
  else
  {
    // Se há bastante luz, apaga a iluminação auxiliar
    analogWrite(ILUMINACAO_AUXILIAR, 0);
  }

  // Exibe informações no monitor serial
  Serial.print("Vagas Livres: ");
  Serial.print(totalLivres);
  Serial.print(" | Ocupadas: ");
  Serial.println(totalOcupadas);

  // Exibe informações no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Livres: ");
  lcd.print(totalLivres);
  lcd.print(" Ocup: ");
  lcd.print(totalOcupadas);

  lcd.setCursor(0, 2);
  lcd.print("V1: ");
  lcd.print(vaga1Ocupada ? "Ocup" : "Livre");
  lcd.print(" V2: ");
  lcd.print(vaga2Ocupada ? "Ocup" : "Livre");

  delay(500);  // Aguarda meio segundo antes da próxima leitura
}