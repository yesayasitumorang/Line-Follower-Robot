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
int maju = 200;
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
      delay(500);  // Tunggu sebentar sebelum melanjutkan
      flag = true;  // Set flag agar perintah case dijalankan
    }

    // Menangani simpang sesuai dengan nilai simpang
    switch(rintangan) {
      case 1://start(maju)
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, majuCepat);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, majuCepat);  // Kecepatan belok maju
        delay(100);
        break;
      case 2://simpang 1 (belok kiri)
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, maju);  // Kecepatan belok maju
        delay(1500);
        break;
      case 4://simpang 2 (ambil pohon dan belok kiri)
        digitalWrite(MOTOR_A_DIR, LOW);
        digitalWrite(MOTOR_B_DIR, LOW);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok maju
        delay(300);
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, majuCepat);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, majuCepat);
        delay(500);
      case 5: //simpang setelah simpang 2
        digitalWrite(MOTOR_A_SPEED, LOW);
        digitalWrite(MOTOR_B_SPEED, LOW);
        analogWrite(MOTOR_A_SPEED, 0);
        analogWrite(MOTOR_B_SPEED, 0);
        delay(400);
        digitalWrite(MOTOR_A_DIR, LOW);
        digitalWrite(MOTOR_B_DIR, LOW);
        analogWrite(MOTOR_A_SPEED, 200);
        analogWrite(MOTOR_B_SPEED, 200);
        delay(300);             
          // Gerakkan servo dari 0° ke 180° dengan langkah kecil
        Serial.println("Servo bergerak dari 0 ke 50 derajat");
        smoothMove(180, 90, 2); // Dari 0° ke 180° dengan langkah 2°
        delay(1000);

        digitalWrite(MOTOR_A_DIR, LOW);
        digitalWrite(MOTOR_B_DIR, LOW);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok maju

        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 180);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 180);  // Kecepatan belok maju
        delay(100);

        digitalWrite(MOTOR_A_DIR, LOW);
        digitalWrite(MOTOR_B_DIR, LOW);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok maju

          // Gerakkan servo dari 180° ke 0° dengan langkah kecil
        Serial.println("Servo kembali dari 180 ke 0 derajat");
        smoothMove(90, 180, 2); // Dari 180° ke 0° dengan langkah 2°
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
      case 7:
        stop();
        delay(100);
        mundur();
        delay(400);
        Serial.println("Servo kembali dari 180 ke 0 derajat");
        smoothMove(90, 180, 2); // Dari 180° ke 0° dengan langkah 2°
        delay(700);
        mundur();
        delay(900);
        belokKanan();
        delay(300);

      /* case ://simpang 3 (belok kanan & taruh object)
        digitalWrite(MOTOR_A_DIR, HIGH);   
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok maju
        delay(1500);
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan belok maju
        delay(500);
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan belok maju
        delay(10);
        break; */
      case 9:// simpang 4 (belok kanan)
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok kanan
        analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan belok kanan
        delay(1000);
        break;
      case 13:
        digitalWrite(MOTOR_A_DIR, LOW);
        digitalWrite(MOTOR_B_DIR, LOW);
        analogWrite(MOTOR_A_SPEED, 0);  // Kecepatan belok kanan
        analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok kanan
        delay(10000000000);
      default:
        Serial.println("Kasus tidak ditangani!");
        break;
    }
  }
   else if (s2 == HIGH && s3 == HIGH && s4 == HIGH) {  // case check point
    if (!sensorAktifSebelumnya) {  // Jika sensor sebelumnya tidak aktif
      rintangan++;  // Tambah simpang
      sensorAktifSebelumnya = true;  // Tandai bahwa sensor sudah aktif
      Serial.print("rintangan: "); Serial.println(rintangan);  // Debugging nilai simpang
    }
    if (!flag) {
      stop();  // Berhenti dulu
      delay(1000);  // Tunggu sebentar sebelum melanjutkan
      flag = true;  // Set flag agar perintah case dijalankan
    }
     switch(rintangan) {
      case 3://checkpoint 1
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, majuCepat);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, majuCepat);  // Kecepatan belok maju
        delay(100);
        break;
      case 6://checkpoint 2
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, belokCepat);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 0);  // Kecepatan belok maju
        delay(300);
        break;
      case 8://checkpoint 3
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, majuCepat);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, majuCepat);  // Kecepatan belok maju
        delay(50);
        break;
  } 
   }
  else if (s3 == HIGH && s4 == HIGH && s5 == HIGH) {  //case belok kanan
    if (!sensorAktifSebelumnya) {  // Jika sensor sebelumnya tidak aktif
      rintangan++;  // Tambah simpang
      sensorAktifSebelumnya = true;  // Tandai bahwa sensor sudah aktif
      Serial.print("rintangan: "); Serial.println(rintangan);  // Debugging nilai simpang
    }
    if (!flag) {
      stop();  // Berhenti dulu
      delay(1000);  // Tunggu sebentar sebelum melanjutkan
      flag = true;  // Set flag agar perintah case dijalankan
    }
     switch(rintangan) {
      case 11://belok kanan 1
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, majuCepat);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, majuCepat);  // Kecepatan belok maju
        delay(100);
        break;
     }
  } else if (s1 == HIGH && s2 == HIGH && s3 == HIGH) {  //case belok kiri
    if (!sensorAktifSebelumnya) {  // Jika sensor sebelumnya tidak aktif
      rintangan++;  // Tambah simpang
      sensorAktifSebelumnya = true;  // Tandai bahwa sensor sudah aktif
      Serial.print("rintangan: "); Serial.println(rintangan);  // Debugging nilai simpang
    }
    if (!flag) {
      stop();  // Berhenti dulu
      delay(300);  // Tunggu sebentar sebelum melanjutkan
      flag = true;  // Set flag agar perintah case dijalankan
    }
     switch(rintangan) {
      case 9://kiri 1
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 0);
        analogWrite(MOTOR_B_SPEED, 255);
        delay(300);
        break;
      case 10://kiri 2
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 255);  // Kecepatan belok maju
        delay(100);
        break;
      case 12:
        digitalWrite(MOTOR_A_DIR, HIGH);
        digitalWrite(MOTOR_B_DIR, HIGH);
        analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan belok maju
        analogWrite(MOTOR_B_SPEED, 255);
        delay(100);
     }
  } else {  // Jika sensor tidak aktif
    sensorAktifSebelumnya = false;  // Reset status sensor
    flag = false;  // Reset flag agar perintah case bisa dijalankan kembali ketika ada simpang baru
  }


  // Logika untuk gerakan robot lainnya (sama seperti sebelumnya)
 if (s1 == LOW && s2 == LOW && s3 == LOW && s4 == LOW && s5 == LOW) {  // Maju jika sensor tengah aktif
    mundur();
  } else if (s3 == HIGH) {  // maju
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
    analogWrite(MOTOR_B_SPEED, 255);
  } else if (s4 == HIGH || s5 == HIGH) {  // Belok kanan jika sensor kanan aktif
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);
    analogWrite(MOTOR_A_SPEED, 255);  // Kecepatan belok kanan
    analogWrite(MOTOR_B_SPEED, 0);
  } else {  // Berhenti jika tidak ada garis
    digitalWrite(MOTOR_A_DIR, LOW);
    digitalWrite(MOTOR_B_DIR, LOW);
    analogWrite(MOTOR_A_SPEED, 200);
    analogWrite(MOTOR_B_SPEED, 200);
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
