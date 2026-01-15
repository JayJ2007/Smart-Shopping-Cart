/*
 ESP8266 Motor Control & Auto-Follow Firmware
 Smart Shopping Cart Project

 Controls:
 - DC motors via motor driver
 - IR sensors for direction detection
 - Ultrasonic sensor for distance measurement
*/

// ================= Configuration =================
#define IR_ACTIVE LOW   // Change to HIGH if IR sensor outputs HIGH on detection

#define BASE_SPEED    750
#define TURN_SPEED    600
#define SAFE_DISTANCE 30
#define STOP_DISTANCE 15
#define MAX_DISTANCE  100

// ================= Motor Pins =================
#define IN1 5    // D1 - Front Left
#define IN2 4    // D2 - Front Right
#define IN3 0    // D3 - Back Left
#define IN4 2    // D4 - Back Right
#define ENA 12   // D6 - Left Motor PWM
#define ENB 13   // D7 - Right Motor PWM

// ================= Sensor Pins =================
#define LEFT_IR_PIN  16   // D0
#define RIGHT_IR_PIN 15   // D8
#define TRIG_PIN     3    // RX
#define ECHO_PIN     1    // TX

long duration;
int distance;

// ================= Setup =================
void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  stopMotors();
  delay(1000);

  Serial.println("Smart Cart Ready (Auto-Follow Mode)");
}

// ================= Main Loop =================
void loop() {
  distance = getDistance();
  int leftIR  = digitalRead(LEFT_IR_PIN);
  int rightIR = digitalRead(RIGHT_IR_PIN);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" | Left IR: ");
  Serial.print(leftIR);
  Serial.print(" | Right IR: ");
  Serial.println(rightIR);

  // ---- Distance-based control ----
  if (distance <= STOP_DISTANCE) {
    stopMotors();
  }
  else if (distance > STOP_DISTANCE && distance < SAFE_DISTANCE) {
    moveForward(BASE_SPEED * 0.5);
  }
  else if (distance >= SAFE_DISTANCE && distance <= MAX_DISTANCE) {
    // ---- Direction correction ----
    if (leftIR == IR_ACTIVE && rightIR != IR_ACTIVE) {
      turnLeft();
    }
    else if (rightIR == IR_ACTIVE && leftIR != IR_ACTIVE) {
      turnRight();
    }
    else {
      moveForward(BASE_SPEED);
    }
  }
  else {
    moveForward(BASE_SPEED + 200);
  }

  delay(100);
}

// ================= Ultrasonic Distance =================
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 25000);
  return duration * 0.034 / 2;
}

// ================= Motor Control =================
void moveForward(int spd) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, TURN_SPEED * 0.6);
  analogWrite(ENB, TURN_SPEED);
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, TURN_SPEED * 0.6);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
