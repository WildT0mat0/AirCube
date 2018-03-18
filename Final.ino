/*
프로그램명: Air Cube 1.0.0
프로그램 소스 제작자: 한동엽(인천하늘고등학교 7기, HAM 7기 기장)
프로그램 소스 제작 일시: 2018/03/12~2018/03/16
본 소스의 저작권은 인천하늘고등학교 정규동아리 HAM에 있으며, 
어떠한 형태로든 수정, 사용, 복제는 저작권자와의 협의 하에 이루어져햐 합니다.
협의 없이 이루어진 이 프로그램에 관련된 모든 행위에 대해 
그 행위의 주체는 불이익을 받을 수 있습니다.
*/

#include <DHT.h>
#include <SoftwareSerial.h>

#define DHTPIN 12
#define DHTTYPE DHT11
#define dustpin 8

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial BT(2, 3);

unsigned long pulse = 0;
float ugm3 = 0;

void setup() {
  pinMode(dustpin, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(11, LOW);
  delay(200);
  pinMode(11, OUTPUT);
  delay(200);
  Serial.begin(9600);
  BT.begin(9600);

  /*
    //CO값 튕기면 2.5분 존버하는 곳
    analogWrite(A7, HIGH);
    delay(60000);
    analogWrite(A7, 71.4);
    delay(90000);
    analogWrite(A7, HIGH);
  */
}

void loop() {
  int temp = dht.readTemperature();
  int hum = dht.readHumidity();
  float uval = isUncomf(temp, hum);

  int CO_val = analogRead(A7);
  double CO_val_log = log10(CO_val);
  Serial.print("CO: ");
  Serial.print(CO_val_log); //상용로그 취하는게 맞는듯. 예제들이 주로 로그 사용. 사용시 1~3ppm
  Serial.print("ppm");
  Serial.print("|");

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("C|");
  Serial.print("Humid: ");
  Serial.print(hum);
  Serial.print("%|");

  pulse = pulseIn(dustpin , LOW, 20000);
  ugm3 = pulse2ugm3(pulse);
  Serial.print("Dust: ");
  Serial.print(ugm3);
  Serial.println("ug/m3|");

  Serial.print("Uncomf_val: ");
  Serial.print(uval);
  Serial.print("|");
  delay(1000);

  if (ugm3<15 && ugm3>=0) {
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  }
  else if (ugm3>=15 && ugm3<50) {
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
  }
  else if (ugm3>= 51 && ugm3<500) {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
  }
  else {
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    delay(500);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    delay(500);
  }

  if (uval < 68 && uval >= 0) {
    analogWrite(A0, 255);
    analogWrite(A1, 0);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
  }
  else if (uval >= 68 && uval < 75) {
    analogWrite(A0, 0);
    analogWrite(A1, 255);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
  }
  else if (uval >= 75 && uval <= 80) {
    analogWrite(A0, 0);
    analogWrite(A1, 0);
    analogWrite(A2, 255);
    analogWrite(A3, 255);
  }
  else if (uval > 80) {
    analogWrite(A0, 255);
    analogWrite(A1, 255);
    analogWrite(A2, 255);
    analogWrite(A3, 255);
  }
  else {
    analogWrite(A0, 0);
    analogWrite(A1, 0);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
  }
}

float pulse2ugm3(unsigned long pulse) {
  float dust_value = (pulse - 1400) / 14.0;
  if (dust_value > 300) {
    dust_value = 0;
  }
  return dust_value;
}

float isUncomf(int temp, int hum) {
  int t = temp;
  int h = hum / 100;
  float uncomf_val = (((t * 9 / 5) - (0.55 * (1 - h)) * (t * 9 / 5 - 26)) + 32);
  return uncomf_val;
}


