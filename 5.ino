#include <QTRSensors.h>

QTRSensors qtr;

// Піни драйвера моторів
#define PWMA 3
#define AIN2 4
#define AIN1 5
#define STBY 6
#define BIN1 7
#define BIN2 8
#define PWMB 9

#define BUTT 12

const uint8_t SensorCount = 8;

// Базова швидкість руху
int minSpeed = 225;

int lastError = 0;

uint16_t sensorValues[SensorCount];

void setup()
{
   // Налаштування пінів моторів
  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(BUTT, INPUT_PULLUP);

  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, 10, 11}, SensorCount);
  qtr.setEmitterPin(13);

  //Serial.begin(9600);

  // Калібрування датчиків
  while(true){
	qtr.calibrate();
    if(digitalRead(BUTT) == 0){
      delay(2000);
      break;
    }
  }
  // Виведення драйвера зі standby
  digitalWrite(STBY, 1);
}

int Placement() {
 
  int position = qtr.readLineBlack(sensorValues);
    position-=3500;
   return position;
}

void SetRapid(int pos, int base_speed) {
  
    int motorSpeed = 0.3 * pos + 1.7 * (pos - lastError);

    lastError = pos;
 
    int rmSpeed = base_speed + motorSpeed;
    int lmSpeed = base_speed - motorSpeed;

    lmSpeed = constrain(lmSpeed, 0, 255);
    rmSpeed = constrain(rmSpeed, 0, 255);

    leftMotor(lmSpeed);
    rightMotor(rmSpeed);
    
}

void loop()
{
  qtr.read(sensorValues);
  int place = Placement();

  SetRapid(place, minSpeed);
}

void rightMotor(byte speed){
	digitalWrite(AIN2, 1);
  digitalWrite(AIN1, 0);
	analogWrite(PWMA, speed);
}

void leftMotor(byte speed){
  digitalWrite(BIN2, 0);
  digitalWrite(BIN1, 1);
	analogWrite(PWMB, speed);
}