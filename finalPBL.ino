// Pin Motor (Direction control)
#define MOTOR_A_DIR D1  // GPIO1
#define MOTOR_B_DIR D2  // GPIO2
#define MOTOR_A_SPEED D3 // GPIO3 (PWM untuk kecepatan)
#define MOTOR_B_SPEED D4 // GPIO4 (PWM untuk kecepatan)

// Pin Sensor (5 channel)
#define SENSOR_1 D0    // Kiri jauh
#define SENSOR_2 D5    // Kiri
#define SENSOR_3 D6    // Tengah
#define SENSOR_4 D7    // Kanan
#define SENSOR_5 D8    // Kanan jauh

int rintangan = 0;  // Variabel untuk menghitung jumlah simpang yang terdeteksi
bool sensorAktifSebelumnya = false;  // Menyimpan status kelima sensor aktif sebelumnya
bool flag = false;  // Flag untuk menandakan robot berhenti sejenak

const int g_maju = 250;
const int g_belok = 220;

void setup() {
 

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
      digitalWrite(MOTOR_A_DIR, LOW);
      digitalWrite(MOTOR_B_DIR, LOW);
      analogWrite(MOTOR_A_SPEED, 0);
      analogWrite(MOTOR_B_SPEED, 0);  // Berhenti dulu
      delay(300);  // Tunggu sebentar sebelum melanjutkan

      flag = true;  // Set flag agar perintah case dijalankan
    }

    // Menangani simpang sesuai dengan nilai simpang
    switch(rintangan) {
      case 1://start(maju)
      Serial.print("rintangan ke -"); Serial.println(rintangan);
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, g_maju);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, g_maju);  // Kecepatan belok maju
        delay(500);
        break;
      case 2://simpang 1 (belok kiri)
      Serial.print("rintangan ke -"); Serial.println(rintangan);
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, g_maju);  // Kecepatan belok maju
        delay(500);
        break;
      case 3://simpang 2 (ambil pohon dan belok kiri)
      Serial.print("rintangan ke -"); Serial.println(rintangan);
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, g_maju);  // Kecepatan belok maju
        delay(500);
        break;
      case 4://simpang 3 (belok kanan & taruh object)
        Serial.print("rintangan ke -"); Serial.println(rintangan);
        digitalWrite(MOTOR_A_DIR, LOW);
        digitalWrite(MOTOR_B_DIR, LOW);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok maju
        delay(2000);
        break;    
      default:
        Serial.println("Kasus tidak ditangani!");
        break;
    }

   
  } else {  // Jika sensor tidak aktif
    sensorAktifSebelumnya = false;  // Reset status sensor
    flag = false;  // Reset flag agar perintah case bisa dijalankan kembali ketika ada simpang baru
  }
  
  // Logika untuk gerakan robot lainnya (sama seperti sebelumnya)
 if (s3 == HIGH) {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan maksimal
  analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan maksimal;
 } else if (s2 == HIGH || s1 == HIGH) {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok kiri
  analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan belok kiri
 } else if (s4 == HIGH || s5 == HIGH) {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan belok kanan
  analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok kanan
 } else if (s1 == HIGH && s2 == HIGH && s3 == HIGH) {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan maksimal
  analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan maksimal;
 } else if (s3 == HIGH && s4 == HIGH && s5 == HIGH) {
  if (rintangan == 3) {
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);
    analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok kiri
    analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan belok kiri
  } else {
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);
    analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan maksimal
    analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan maksimal;
  }
  delay(500);  
 } else {
  mundur();
  delay(1000);
 }
}

// Fungsi Gerakan Motor
void maju() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan maksimal
  analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan maksimal
}

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
