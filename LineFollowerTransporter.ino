#include <Servo.h>

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
int cekpoint = 0; //variabel untuk menghitung jumlah cekpoint yang terdeteksi
bool sensorAktifSebelumnya = false;  // Menyimpan status kelima sensor aktif sebelumnya
bool flag = false;  // Flag untuk menandakan robot berhenti sejenak
int pos = 0;
int posisiawal = 90;

const int g_maju = 255;
const int g_belok = 200;

void setup() {
 
  myServo.attach(D7);

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
  myServo.write(posisiawal);
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
      digitalWrite(MOTOR_A_DIR, LOW);
      digitalWrite(MOTOR_B_DIR, LOW);
      analogWrite(MOTOR_A_SPEED, 0);
      analogWrite(MOTOR_B_SPEED, 0);  // Berhenti dulu
      delay(500);  // Tunggu sebentar sebelum melanjutkan
      flag = true;  // Set flag agar perintah case dijalankan
    }

    // Menangani simpang sesuai dengan nilai simpang
    switch(rintangan) {
      case 1://start(maju)
        Serial.print("rintangan ke -"); Serial.println(rintangan);
        maju();
        delay(500);
        break;
      case 2://simpang 1 (belok kiri)
        Serial.print("rintangan ke -"); Serial.println(rintangan);
        belokKiri();
        delay(500);
        break;
      case 3://simpang 2 (ambil pohon dan belok kiri)
        Serial.print("rintangan ke -"); Serial.println(rintangan);
        maju(); // Kecepatan belok maju
        delay(200);
        break;
      case 4: 
        Serial.print("rintangan ke -"); Serial.println(rintangan);
        stop();
        delay(300);
        mundur();
        delay(500);


      case 5://simpang 3 (belok kanan & taruh object)
        Serial.print("rintangan ke -"); Serial.println(rintangan);
        berhenti();  // Kecepatan belok maju
        delay(500);
        break;    
      default:
        Serial.println("Kasus tidak ditangani!");
        break;
    }

   
  } else if (s2 == HIGH && s3 == HIGH && s4 == HIGH) {
    cekpoint++;
    Serial.print("cekpoint : "); Serial.println(cekpoint);
    switch(cekpoint) {
      case 1:
        Serial.print("cekpoint ke - "); Serial.println(cekpoint);
        maju();
        delay(500);
        break;
      case 2:
        Serial.print("cekpoint ke - "); Serial.println(cekpoint);
        belokKanan();
        delay(500);
        break;
      case 3:
        maju();
        delay(500);
    }
  } else {  // Jika sensor tidak aktif
    sensorAktifSebelumnya = false;  // Reset status sensor
    flag = false;  // Reset flag agar perintah case bisa dijalankan kembali ketika ada simpang baru
  }

  
  // Logika untuk gerakan robot lainnya (sama seperti sebelumnya)
 if (s3 == HIGH) {
  delay(10);
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, g_maju);  // Kecepatan maksimal
  analogWrite(MOTOR_B_SPEED, g_maju);  // Kecepatan maksimal;
 } else if (s2 == HIGH || s1 == HIGH) {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok kiri
  analogWrite(MOTOR_B_SPEED, g_maju);  // Kecepatan belok kiri
 } else if (s4 == HIGH || s5 == HIGH) {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, g_maju);  // Kecepatan belok kanan
  analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok kanan
 } else if (s1 == HIGH && s2 == HIGH && s3 == HIGH) {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, g_maju);  // Kecepatan maksimal
  analogWrite(MOTOR_B_SPEED, g_maju);  // Kecepatan maksimal;
 } else if (s3 == HIGH && s4 == HIGH && s5 == HIGH) {
  if (rintangan == 3) {
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);
    analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok kiri
    analogWrite(MOTOR_B_SPEED, g_maju);  // Kecepatan belok kiri
  } else {
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);
    analogWrite(MOTOR_A_SPEED, g_maju);  // Kecepatan maksimal
    analogWrite(MOTOR_B_SPEED, g_maju);  // Kecepatan maksimal;
  }
  delay(90);  
 } else {
  mundur();
  delay(500);
 }
}

// Fungsi Gerakan Motor
void maju() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, g_maju);  // Kecepatan maksimal
  analogWrite(MOTOR_B_SPEED, g_maju);  // Kecepatan maksimal
}

void belokKiri() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok kiri
  analogWrite(MOTOR_B_SPEED, g_maju);  // Kecepatan belok kiri
}

void belokKanan() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, g_maju);  // Kecepatan belok kanan
  analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok kanan
}

void stop() {
  digitalWrite(MOTOR_A_DIR, LOW);
  digitalWrite(MOTOR_B_DIR, LOW);
  analogWrite(MOTOR_A_SPEED, 0);
  analogWrite(MOTOR_B_SPEED, 0); // Berhenti dengan mematikan motor
}

void mundur() {
  analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan mundur
  analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan mundur
}

//fungsi servo
void buka() {
  for (pos == 0; pos <= 180; pos += 20) {
    myServo.write(pos)
  }
}

void tutup() {
  for (pos == 180; pos <= 180; pos += 20) {
    myServo.write(pos)
  }
}
