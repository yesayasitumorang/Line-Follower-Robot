// Pin Motor (Direction control)
#define MOTOR_A_DIR D3 // GPIO1
#define MOTOR_B_DIR D4 // GPIO2
#define MOTOR_A_SPEED D1 // GPIO3 (PWM untuk kecepatan)
#define MOTOR_B_SPEED D2               // GPIO4 (PWM untuk kecepatan)

// Pin Sensor (5 channel)
#define SENSOR_1 D0   // Kiri jauh
#define SENSOR_2 D5   // Kiri
#define SENSOR_3 D6   // Tengah
#define SENSOR_4 D7   // Kanan
#define SENSOR_5 D8   // Kanan jauh
// #define NEAR     10



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
  // pinMode(NEAR    ,  INPUT);

  // Inisialisasi komunikasi serial untuk debugging
  Serial.begin(115200);
}

void loop() {
  // Membaca nilai sensor
  int s1 = digitalRead(SENSOR_1);  // Kiri jauh
  int s2 = digitalRead(SENSOR_2);  // Kiri
  int s3 = digitalRead(SENSOR_3);  // Tengah
  int s4 = digitalRead(SENSOR_4);  // Kanan
  int s5 = digitalRead(SENSOR_5);  // Kanan jauh
  // int s6 = digitalRead(NEAR);

  // Keputusan berdasarkan sensor
  if (s3 == HIGH) { 
    maju();
  } else if (s1 == HIGH || s2 == HIGH) {  
    belokKiri();
  } else if (s4 == HIGH || s5 == HIGH) {  
    belokKanan();
  } else if (s4 == LOW && s5 == LOW) { 
    belokkanantajam();
  } else if (s1 == LOW && s2 == LOW) {
    belokkiritajam();
  } 
  
  // if (s6 == HIGH) {
  //   berhenti();
  // }
  // Debug output
  Serial.print(" S1: "); Serial.print(s1);
  Serial.print(" S2: "); Serial.print(s2);
  Serial.print(" S3: "); Serial.print(s3);
  Serial.print(" S4: "); Serial.print(s4);
  Serial.print(" S5: "); Serial.println(s5);
  // Serial.print(" S6: "); Serial.print(s6);
  
  delay(100); // Delay untuk stabilitas pembacaan
}

// Fungsi Gerakan Motor
void maju() {
  digitalWrite(MOTOR_A_DIR, LOW);
  digitalWrite(MOTOR_B_DIR, LOW);
  analogWrite(MOTOR_A_SPEED, 255);
  analogWrite(MOTOR_B_SPEED, 255);
}

void belokKiri() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, LOW);
  analogWrite(MOTOR_A_SPEED, 150);
  analogWrite(MOTOR_B_SPEED, 255);
}

void belokKanan() {
  digitalWrite(MOTOR_A_DIR, LOW);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 255);
  analogWrite(MOTOR_B_SPEED, 150);
}

void belokkanantajam() {
  digitalWrite(MOTOR_A_DIR, LOW);
  digitalWrite(MOTOR_B_DIR, HIGH);
  analogWrite(MOTOR_A_SPEED, 150);
  analogWrite(MOTOR_B_SPEED, 100);
}

void belokkiritajam() {
  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, LOW);
  analogWrite(MOTOR_A_SPEED, 100);
  analogWrite(MOTOR_B_SPEED, 150);
}

void berhenti() {
  digitalWrite(MOTOR_A_DIR, LOW);
  digitalWrite(MOTOR_B_DIR, LOW);
  analogWrite(MOTOR_A_SPEED, 0);
  analogWrite(MOTOR_B_SPEED, 0);
  
  

}
