#include <Servo.h>

Servo servoku;
//Servo servoku1;

const int SENSOR_1 = 16;
const int SENSOR_2 = 14;
const int SENSOR_3 = 12;
const int SENSOR_4 = 10;
const int SENSOR_5 = 9;

#define MOTOR_A_DIR D1
#define MOTOR_B_DIR D2
#define PWM_A D3
#define PWM_B D4

#define servo D7
//#define servo1 D8

int rintangan = 0;  // Variabel untuk menghitung jumlah simpang yang terdeteksi
bool sensorAktifSebelumnya = false;  // Menyimpan status kelima sensor aktif sebelumnya
bool flag = false;  // Flag untuk menandakan robot berhenti sejenak
int majuCepat = 255;
int belokCepat = 200;
int posisiServo = 0;

void setup() {
  servoku.attach(servo);
  //servoku.attach(servo1);

  //konfigurasi pin OUTPUT MOTOR DC
  pinMode(MOTOR_A_DIR, OUTPUT);
  pinMode(MOTOR_B_DIR, OUTPUT);
  pinMode(PWM_A, OUTPUT);
  pinMode(PWM_B, OUTPUT);

  //konfigurasi pin OUTPUT sensor
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  pinMode(SENSOR_3, INPUT);
  pinMode(SENSOR_4, INPUT);
  pinMode(SENSOR_5, INPUT);

  Serial.begin(9600);


  //servoku1.write(180);
  //servoku.write(180);
  //delay(1000);
 
}

void loop() {

  //mengubah ke variabel lain
  int s1 = digitalRead(SENSOR_1);  // Kiri jauh
  int s2 = digitalRead(SENSOR_2);  // Kiri
  int s3 = digitalRead(SENSOR_3);  // Tengah
  int s4 = digitalRead(SENSOR_4);  // Kanan
  int s5 = digitalRead(SENSOR_5);  // Kanan jauh

  // Debugging nilai sensor
  Serial.print("S1: "); Serial.print(s1);
  Serial.print(" S2: "); Serial.print(s2);
  Serial.print(" S3: "); Serial.print(s3);
  Serial.print(" S4: "); Serial.print(s4);
  Serial.print(" S5: "); Serial.println(s5);

  //case simpang
  if (s1 == HIGH && s2 == HIGH && s3 == HIGH && s4 == HIGH && s5 == HIGH) {
    if (!sensorAktifSebelumnya) {
      rintangan++;
      sensorAktifSebelumnya = true;
      Serial.print("Rintangan : - "); Serial.println(rintangan);
    }

    if (!flag) {
      stop();
      delay(400);
      flag = true;
    }

    switch (rintangan) {
      case 1: //pada simpang1 adalah awal, sehingga robot maju
        majuKuat();
        delay(1000);
        break;
      case 2: // belok kanan
        belokKanan();
        delay(600);
        break;
      case 3: // angkat benda
        stop();
        delay(900);
        majuKuat();
        delay(100);
        stop();
        delay(2000);
        //bukaServo();
        servoku.write(180);
        delay(500);
        majuKuat();
        delay(900);
        stop();
        delay(2000);
        servoku.write(0);
        delay(1000);
        mundur();
        mundur();
        mundur();
        delay(2000);
        belokKanan();
        delay(800);
        majuKuat();
        delay(300);
        break;
      case 4:
        belokKiri();
        delay(700);
        majuKuat();
        delay(700);
      case 5:
        stop();
        delay(200);
        mundur();
        delay(300);
        servoku.write(180);
        delay(3000);
        mundur();
        delay(900);
        belokKanan();
        delay(1000);
        majuKuat();
        delay(600);
        break;
      case 6:
        belokKanan();
        delay(300);
      case 7:
        belokKanan();
        delay(300);
      case 8:
        belokKanan();
        delay(300);
      default:
        Serial.println("Tindakan selesai");
        break;

    }
  } else {
    sensorAktifSebelumnya = false;  // Reset status sensor
    flag = false;  // Reset flag agar perintah case bisa dijalankan kembali ketika ada simpang baru
  }

  //deteksi garis dengan sensor (logika if-else)
  if (s3 == HIGH) {
    majuKuat();
  } else if (s1 == HIGH && s5 == HIGH) {
    majuKuat();
  } else if (s1 == HIGH || s2 == HIGH) {
    belokKiri();
  } else if (s4 == HIGH || s5 == HIGH) {
    belokKanan();
  } else if (s1 == HIGH || s5 == HIGH) {
    majuKuat();
  } else if(s1 == HIGH && s2 == HIGH && s3 == HIGH && s4 == HIGH) {
    belokKiri();
  } else if (s2 == HIGH && s3 == HIGH && s4 == HIGH && s5 == HIGH ) {
    belokKanan();
  } else if (s1 == HIGH && s2 == HIGH && s3 == HIGH) {
    belokKiri();
    delay(300);
  } else if (s3 == HIGH && s4 == HIGH && s5 == HIGH) {
    belokKanan();
    delay(300);
  } else if(s1 == HIGH && s2 == HIGH) {
    belokKiri();
  } else if(s4 == HIGH && s5 == HIGH) {
    belokKanan();
  } else {
    mundur();
  }

  delay(100);
}

//fungsi gerak robot
void mundur() {
  digitalWrite(MOTOR_A_DIR, LOW);  // Motor A bergerak mundur
  digitalWrite(MOTOR_B_DIR, LOW);  // Motor B bergerak mundur
  analogWrite(PWM_A, 250);        // Kecepatan motor A
  analogWrite(PWM_B, 250);
}

void majuKuat() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(PWM_A, 250);
  analogWrite(PWM_B, 250);
}

void majuLemah() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(PWM_A, 220);
  analogWrite(PWM_B, 220);
}

void belokKanan() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(PWM_A, 255);
  analogWrite(PWM_B, 0);
}

void belokKiri() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(PWM_A, 0);
  analogWrite(PWM_B, 255);
}

void stop() {
  digitalWrite(MOTOR_A_DIR, LOW);
  digitalWrite(MOTOR_B_DIR, LOW);
  analogWrite(PWM_A, 0);
  analogWrite(PWM_B, 0);
}
