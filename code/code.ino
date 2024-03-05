#include <QTRSensors.h> // QTR Kütüphanesi 3.0.0 veya 3.1.0 Kullanılmalıdır.
#define led_buzzer 13 // Led Ve Buzzer Pini.
#define solmotor1 3 // Sol Motor PWM 1
#define solmotor2 11 // Sol Mtor PWM 2
#define sagmotor1 9 // Sağ Motor PWM 1
#define sagmotor2 10 // Sağ Motor PWM 2
int i = 0; 
QTRSensorsAnalog qtra((unsigned char[]) {A3, A2 , A1, A0}, 4, 4); // Sensörlerin Bağlı Olduğu Pin Numaraları.
unsigned int sensorValues[4]; // Sensör Sayısı.

int tabanhiz = 255; // Hata Değeri 0'da Uygulanacak Hız Değeri (Max. 255 PWM).
int maxhiz = 255; // Motor CW/CCW Max Değer.
float Kp = 0.18;
float Kd = 3;
int sonhata = 0;
int hata = 0;
int sagmotorpwm = 0; // Başlangıç PWM Değeri.
int solmotorpwm = 0; // Başlangıç PWM Değeri.
int zemin = 0; // Siyah Zemin Beyaz Çizgi = 1, Beyaz Zemin Siyah Çizgi = 0.

void setup() {
  pinMode(solmotor1, OUTPUT);
  pinMode(solmotor2, OUTPUT);
  pinMode(sagmotor1, OUTPUT);
  pinMode(sagmotor2, OUTPUT);
  tone(led_buzzer, 500, 100);
  delay(50);
  noTone(led_buzzer);
  Serial.begin(9600);

  for (i = 0; i < 80; i++)
  {
    if (0 <= i && i < 5)  motorkontrol(-150, 150);
    if (5 <= i && i  < 15) motorkontrol(150, -150);
    if (15 <= i && i < 25)  motorkontrol(-150, 150);
    if (25 <= i && i < 35)  motorkontrol(150, -150);
    if (35 <= i && i < 45)  motorkontrol(-150, 150);
    if (45 <= i && i  < 55) motorkontrol(150, -150);
    if (55 <= i && i < 65)  motorkontrol(-150, 150);
    if (65 <= i && i < 70)  motorkontrol(150, -150);
    if ( i >= 70  )  {
      motorkontrol(0, 0);
      delay(5);
    }
    qtra.calibrate();
    delay(3);
  }

  motorkontrol(0, 0);
  tone(led_buzzer, 3000, 100);
  delay(50);
  noTone(led_buzzer);
}


void loop() {
    hata = ( qtra.readLine(sensorValues, 1, zemin) - 1500);
    int duzeltmehizi = Kp * hata + Kd * (hata - sonhata);
    sonhata = hata;
    solmotorpwm = tabanhiz + duzeltmehizi  ;
    sagmotorpwm = tabanhiz - duzeltmehizi  ;
    solmotorpwm = constrain(solmotorpwm, -255, maxhiz); 
    sagmotorpwm = constrain(sagmotorpwm, -255, maxhiz);
    motorkontrol(sagmotorpwm, solmotorpwm);
}


void motorkontrol(int sagmotorpwm, int solmotorpwm) {
  if (sagmotorpwm <= 0) {
    sagmotorpwm = abs(sagmotorpwm);
    analogWrite(sagmotor1, 0);
    analogWrite(sagmotor2, sagmotorpwm);
  } else {
    analogWrite(sagmotor1, sagmotorpwm);
    analogWrite(sagmotor2, 0);
  }

  if (solmotorpwm <= 0) {
    solmotorpwm = abs(solmotorpwm);
    analogWrite(solmotor1, solmotorpwm);
    analogWrite(solmotor2, 0);
  } else {
    analogWrite(solmotor1, 0);
    analogWrite(solmotor2, solmotorpwm);
  }

  // Print direction information to Serial Monitor
  if (sagmotorpwm > 0 && solmotorpwm > 0) {
    Serial.println("Forward");
  } else if (sagmotorpwm < 0 && solmotorpwm < 0) {
    Serial.println("Backward");
  } else if (sagmotorpwm > 0 && solmotorpwm == 0) {
    Serial.println("Right");
  } else if (sagmotorpwm == 0 && solmotorpwm > 0) {
    Serial.println("Left");
  }
}
