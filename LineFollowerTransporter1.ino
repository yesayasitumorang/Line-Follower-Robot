#include <Servo.h>

#define servo D7

Servo myServo;


// Pin Motor (Direction control)
#define MOTOR_A_DIR D1  // GPIO1
#define MOTOR_B_DIR D2  // GPIO2
#define MOTOR_A_SPEED D3 // GPIO3 (PWM untuk kecepatan)
#define MOTOR_B_SPEED D4 // GPIO4 (PWM untuk kecepatan)

// Pin Sensor (5 channel)
const int SENSOR_1 = 16;
const int SENSOR_2 = 14;
const int SENSOR_3 = 12;
const int SENSOR_4 = 10;
const int SENSOR_5 = 9;

int rintangan = 0;  // Variabel untuk menghitung jumlah simpang yang terdeteksi
bool sensorAktifSebelumnya = false;  // Menyimpan status kelima sensor aktif sebelumnya
bool flag = false;  // Flag untuk menandakan robot berhenti sejenak
int majuCepat = 250;
int belokCepat = 200;
int belok = 180;

void setup() {
  myServo.attach(servo); // Hubungkan servo ke GPIO9
  Serial.println("servo bergerak");

  // Konfigurasi pin motor sebagai output
  pinMode(MOTOR_A_DIR, OUTPUT);
  pinMode(MOTOR_B_DIR, OUTPUT);
  pinMode(MOTOR_A_SPEED, OUTPUT);
  pinMode(MOTOR_B_SPEED, OUTPUT);

  // Konfigurasi pin sensor sebagai input
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  pinMode(SENSOR_3, INPUT);
  pinMode(SENSOR_4, INPUT);
  pinMode(SENSOR_5, INPUT);

  // Inisialisasi komunikasi serial untuk debugging
  Serial.begin(9600);
}

void loop() {
  // Membaca nilai sensor
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
  
  // case simpang
  if (s1 == HIGH && s2 == HIGH && s3 == HIGH && s4 == HIGH && s5 == HIGH) {
    if (!sensorAktifSebelumnya) {  // Jika sensor sebelumnya tidak aktif
      rintangan++;  // Tambah simpang
      sensorAktifSebelumnya = true;  // Tandai bahwa sensor sudah aktif
      Serial.print("Rintangan: "); Serial.println(rintangan);  // Debugging nilai simpang
    }

    // Menambahkan flag untuk berhenti sejenak sebelum menjalankan case
    if (!flag) {
      stop();  // Berhenti dulu
      delay(300);  // Tunggu sebentar sebelum melanjutkan
      flag = true;  // Set flag agar perintah case dijalankan
    }

    // Menangani simpang sesuai dengan nilai simpang
    switch(rintangan) {
      case 1://start(maju)
        stop();
        delay(1000);
        smoothMove(0, 180, 5); // Dari 0° ke 180° dengan langkah 2°
        delay(1500);
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, majuCepat);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, majuCepat);  // Kecepatan belok maju
        delay(500);
        break;
      case 2://simpang 1 (belok kiri)
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 250);  // Kecepatan belok maju
        delay(500);
        break;
      case 3://simpang 2 (ambil pohon dan belok kiri)
        digitalWrite(MOTOR_A_DIR, LOW);
        digitalWrite(MOTOR_B_DIR, LOW);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok maju
        delay(1000);
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 250);  
        analogWrite(MOTOR_B_SPEED, 250);
        delay(100);
        majuLambat();
        delay(2000);
        analogWrite(MOTOR_A_SPEED, 0);
        analogWrite(MOTOR_B_SPEED, 0);
        delay(700);
        digitalWrite(MOTOR_A_DIR, LOW);
        digitalWrite(MOTOR_B_DIR, LOW);
        analogWrite(MOTOR_A_SPEED, 200);
        analogWrite(MOTOR_B_SPEED, 200);
        delay(100);             
          // Gerakkan servo dari 0° ke 180° dengan langkah kecil
        Serial.println("Servo bergerak dari 0 ke 180 derajat");
        smoothMove(0, 180, 2); // Dari 0° ke 180° dengan langkah 2°
        delay(1000);

        digitalWrite(MOTOR_A_DIR, LOW);
        digitalWrite(MOTOR_B_DIR, LOW);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok maju
        delay(1000);

          // Gerakkan servo dari 180° ke 0° dengan langkah kecil
        Serial.println("Servo kembali dari 180 ke 0 derajat");
        smoothMove(180, 0, 2); // Dari 180° ke 0° dengan langkah 2°
        delay(1000);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok maju
        delay(1000);
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan belok maju
        delay(700);
        break;
      case 4:
        stop();
        delay(600);
        mundur();
        delay(200);
        Serial.println("Servo kembali dari 180 ke 0 derajat");
        smoothMove(0, 180, 2); // Dari 180° ke 0° dengan langkah 2°
        delay(1000);
        mundur();
        delay(1000);
        belokKanan();
        delay(300);
      case 5:
        digitalWrite(MOTOR_A_DIR, LOW);
        digitalWrite(MOTOR_B_DIR, LOW);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok kanan
        analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok kanan
      default:
        Serial.println("Kasus tidak ditangani!");
        break;
    }
  } else {  // Jika sensor tidak aktif
    sensorAktifSebelumnya = false;  // Reset status sensor
    flag = false;  // Reset flag agar perintah case bisa dijalankan kembali ketika ada simpang baru
  }


  // Logika untuk gerakan robot lainnya (sama seperti sebelumnya)
 if (s3 == HIGH) {  // maju
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);
    analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan maksimal
    analogWrite(MOTOR_B_SPEED, 255);  
  } else if (s1 == HIGH && s5 == HIGH) {  // maju
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);
    analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan maksimal
    analogWrite(MOTOR_B_SPEED, 255);
  } else if (s1 == HIGH || s2 == HIGH) {  // Belok kiri jika sensor kiri aktif
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);
    analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok kiri
    analogWrite(MOTOR_B_SPEED, 240);
  } else if (s4 == HIGH || s5 == HIGH) {  // Belok kanan jika sensor kanan aktif
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);
    analogWrite(MOTOR_A_SPEED, 240);
    analogWrite(MOTOR_B_SPEED, 0);
  } else if (s3 == HIGH && s4 == HIGH && s5 == HIGH) {
    if (rintangan == 3 || rintangan == 4) {
      belokKanan();
      delay(400);
    } else {
      digitalWrite(MOTOR_A_DIR, HIGH);
      digitalWrite(MOTOR_B_DIR, HIGH);
      analogWrite(MOTOR_A_SPEED, majuCepat);  // Kecepatan belok kanan
      analogWrite(MOTOR_B_SPEED, majuCepat);
    }
  } else if (s1 == HIGH && s2 == HIGH && s3 == HIGH) {
      maju();
      delay(100);
  } else {  // Berhenti jika tidak ada garis
    mundur();
    delay(1000);
  }
}

/* // Fungsi Gerakan Motor
void maju() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan maksimal
  analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan maksimal
}

 */
 void belokKiri() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok kiri
  analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan belok kiri
}

void belokKanan() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan belok kanan
  analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok kanan
}

 void mundur() {
  digitalWrite(MOTOR_A_DIR, LOW);
  digitalWrite(MOTOR_B_DIR, LOW);
  analogWrite(MOTOR_A_SPEED, 200);
  analogWrite(MOTOR_B_SPEED, 200); // Berhenti dengan mematikan motor
}

void stop() {
  digitalWrite(MOTOR_A_DIR, LOW);
  digitalWrite(MOTOR_B_DIR, LOW);
  analogWrite(MOTOR_A_SPEED, 0);
  analogWrite(MOTOR_B_SPEED, 0); // Berhenti dengan mematikan motor
}

void maju() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, majuCepat);
  analogWrite(MOTOR_B_SPEED, majuCepat);
}

void majuLambat() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 180);
  analogWrite(MOTOR_B_SPEED, 180);
}

void smoothMove(int startAngle, int endAngle, int step) {
  if (startAngle < endAngle) {
    for (int angle = startAngle; angle <= endAngle; angle += step) {
      myServo.write(angle);  // Set posisi servo
      delay(20);             // Tunggu sebelum melangkah ke sudut berikutnya
    }
  } else {
    for (int angle = startAngle; angle >= endAngle; angle -= step) {
      myServo.write(angle);  // Set posisi servo
      delay(20);             // Tunggu sebelum melangkah ke sudut berikutnya
    }
  }
}
