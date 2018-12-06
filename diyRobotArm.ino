#include <Servo.h>

#define ERROR_MARGIN 20

#define BASE_RATIO 0.005
#define CLAW_RATIO 0.005
#define ARM1_RATIO 0.005
#define ARM2_RATIO 0.005

#define DEFAULT_BASE_POS 90
#define DEFAULT_CLAW_POS 180
#define DEFAULT_ARM1_POS 90
#define DEFAULT_ARM2_POS 90

const int SW1_pin = 2;
const int SW2_pin = 3;
const int X1_pin = 0;
const int Y1_pin = 1;
const int X2_pin = 2;
const int Y2_pin = 3;

const int BASE_pin = 8;
const int ARM1_pin = 9;
const int ARM2_pin = 10;
const int CLAW_pin = 11;

Servo BASE_servo;
Servo ARM1_servo;
Servo ARM2_servo;
Servo CLAW_servo;

int BASE_pos = DEFAULT_BASE_POS;
int ARM1_pos = DEFAULT_ARM1_POS;
int ARM2_pos = DEFAULT_ARM2_POS;
int CLAW_pos = DEFAULT_CLAW_POS;

int BASE_read = 0;
int CLAW_read = 0;
int ARM1_read = 0;
int ARM2_read = 0;

const int MIN_pos = 0;
const int MAX_pos = 180;

void setup() {
  // put your setup code here, to run once:
  pinMode(SW1_pin, INPUT);
  pinMode(SW2_pin, INPUT);
  digitalWrite(SW1_pin, HIGH);
  digitalWrite(SW2_pin, HIGH);

  // Attach servos to pins
  BASE_servo.attach(BASE_pin);
  ARM1_servo.attach(ARM1_pin);
  ARM2_servo.attach(ARM2_pin);
  CLAW_servo.attach(CLAW_pin);

  // Initialize servos to base positions:
  BASE_servo.write(BASE_pos);
  ARM1_servo.write(ARM1_pos);
  ARM2_servo.write(ARM2_pos);
  CLAW_servo.write(CLAW_pos);

  BASE_read = analogRead(X1_pin);
  CLAW_read = analogRead(Y1_pin);
  ARM1_read = analogRead(X2_pin);
  ARM2_read = analogRead(Y2_pin);
  
  Serial.begin(115200);
  Serial.println("Calibration complete!");
}

int moveServo(Servo servo, int deviation, int pos, float ratio) {

  if (deviation > ERROR_MARGIN || deviation < -ERROR_MARGIN) {
    int movement = deviation * ratio;
    int newPosition = pos - movement;
    if (newPosition < MAX_pos && newPosition > MIN_pos) {
      servo.write(newPosition);
      return newPosition;
    }
  }
  
  return pos;
}

int resetServo(Servo servo, int pos) {
  servo.write(pos);
  return pos;
}

void loop() {
  // put your main code here, to run repeatedly:

  BASE_pos = moveServo(BASE_servo, analogRead(X1_pin) - BASE_read, BASE_pos, BASE_RATIO);
  CLAW_pos = moveServo(CLAW_servo, analogRead(Y1_pin) - CLAW_read, CLAW_pos, CLAW_RATIO);
  ARM1_pos = moveServo(ARM1_servo, analogRead(X2_pin) - ARM1_read, ARM1_pos, ARM1_RATIO);
  ARM2_pos = moveServo(ARM2_servo, analogRead(Y2_pin) - ARM2_read, ARM2_pos, ARM2_RATIO);
  
  if (digitalRead(SW1_pin) == LOW) {
    BASE_pos = resetServo(BASE_servo, DEFAULT_BASE_POS);
    CLAW_pos = resetServo(CLAW_servo, DEFAULT_CLAW_POS);
  }

  if (digitalRead(SW2_pin) == LOW) {
    ARM1_pos = resetServo(ARM1_servo, DEFAULT_ARM1_POS);
    ARM2_pos = resetServo(ARM2_servo, DEFAULT_ARM2_POS);
  }

  delay(15);
}
