#include <AFMotor.h>
#include <Servo.h>

#define left A3
#define left_center A2
#define right_center A1
#define right A0

#define trigger A4
#define echo A5

AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);


float Kp = 30;
uint8_t multiP = 2;
double steeringvalue;
int position, P, previousError, error;
int lspeed, rspeed;
int speed = 255;
unsigned int steering_time = 500;
unsigned int time;
short state = 1;
int S1, S2, S3, S4;
short checking = 0;
short servoPin = 9;
Servo servo;
bool object_being_checked = false;
unsigned int echo_time;
unsigned int checking_time = 0;
int object_location = 0;
int distance = 0;
int pos = 0;

void readLightsensor() {
    S1 = analogRead(left) > 200 ? 1 : 0;
    S2 = analogRead(left_center) > 200 ? 1 : 0;
    S3 = analogRead(right_center) > 200 ? 1 : 0;
    S4 = analogRead(right) > 200 ? 1 : 0;
}
void go_straight(int rspeed, int lspeed)
{
  motor1.run(FORWARD);
  motor1.setSpeed(rspeed);
  motor2.run(FORWARD);
  motor2.setSpeed(rspeed);
  motor3.run(FORWARD);
  motor3.setSpeed(lspeed);
  motor4.run(FORWARD);
  motor4.setSpeed(lspeed);
}
void go_back(int rspeed, int lspeed)
{
  motor1.run(BACKWARD);
  motor1.setSpeed(rspeed);
  motor2.run(BACKWARD);
  motor2.setSpeed(rspeed);
  motor3.run(BACKWARD);
  motor3.setSpeed(lspeed);
  motor4.run(BACKWARD);
  motor4.setSpeed(lspeed);
}
void strong_left_steering(int rspeed, int lspeed)
{
  motor1.run(FORWARD);
  motor1.setSpeed(rspeed);
  motor2.run(FORWARD);
  motor2.setSpeed(rspeed);
  motor3.run(BACKWARD);
  motor3.setSpeed(lspeed);
  motor4.run(BACKWARD);
  motor4.setSpeed(lspeed);
}
void strong_right_steering(int rspeed, int lspeed)
{
  motor1.run(BACKWARD);
  motor1.setSpeed(rspeed);
  motor2.run(BACKWARD);
  motor2.setSpeed(rspeed);
  motor3.run(FORWARD);
  motor3.setSpeed(lspeed);
  motor4.run(FORWARD);
  motor4.setSpeed(lspeed);
}
void stop()
{
      motor1.run(FORWARD);
      motor1.setSpeed(0);
      motor2.run(FORWARD);
      motor2.setSpeed(0);
      motor3.run(FORWARD);
      motor3.setSpeed(0);
      motor4.run(FORWARD);
      motor4.setSpeed(0);
}


void line_tracking() {
    while (S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0) {
        readLightsensor();
        if (previousError < 0) {
            motor1.run(FORWARD);
            motor1.setSpeed(200);
            motor2.run(FORWARD);
            motor2.setSpeed(200);
            motor3.run(BACKWARD);
            motor3.setSpeed(200);
            motor4.run(BACKWARD);
            motor4.setSpeed(200);
        } else {
            motor1.run(BACKWARD);
            motor1.setSpeed(200);
            motor2.run(BACKWARD);
            motor2.setSpeed(200);
            motor3.run(FORWARD);
            motor3.setSpeed(200);
            motor4.run(FORWARD);
            motor4.setSpeed(200);
        }
    }
    position = (10 * (0 * S1 + 3 * S2 + 4 * S3 + 7 * S4)) / (S1 + S2 + S3 + S4);
    error = position - 35;
    P = error;
    steeringvalue = (Kp / pow(10, multiP)) * P;
    previousError = error;

    lspeed = speed + steeringvalue;
    rspeed = speed - steeringvalue;
    if (rspeed > lspeed) {
        lspeed = 0;
        rspeed = speed;
    } else if (rspeed < lspeed) {
        lspeed = speed;
        rspeed = 0;
    }
    motor1.run(FORWARD);
    motor1.setSpeed(rspeed);
    motor2.run(FORWARD);
    motor2.setSpeed(rspeed);
    motor3.run(FORWARD);
    motor3.setSpeed(lspeed);
    motor4.run(FORWARD);
    motor4.setSpeed(lspeed);
}

void corner_steering() {
    time = millis();
    if (S1 == 1) {
      // trai
        while (millis() - time < steering_time) {
            motor1.run(FORWARD);
            motor1.setSpeed(200);
            motor2.run(FORWARD);
            motor2.setSpeed(200);
            motor3.run(BACKWARD);
            motor3.setSpeed(200);
            motor4.run(BACKWARD);
            motor4.setSpeed(200);
        }
        time = millis();
    } else if (S4 == 1) {
      // phai
        while (millis() - time < steering_time) {
            motor1.run(BACKWARD);
            motor1.setSpeed(200);
            motor2.run(BACKWARD);
            motor2.setSpeed(200);
            motor3.run(FORWARD);
            motor3.setSpeed(200);
            motor4.run(FORWARD);
            motor4.setSpeed(200);
        }
        time = millis();
    }
    while (millis() - time < 50) {
        motor1.run(FORWARD);
        motor1.setSpeed(speed);
        motor2.run(FORWARD);
        motor2.setSpeed(speed);
        motor3.run(FORWARD);
        motor3.setSpeed(speed);
        motor4.run(FORWARD);
        motor4.setSpeed(speed);
    }
}

void control() {
    while (millis() - time < 2000 && state == 1) {
        readLightsensor();
        line_tracking();
    }
    while (state == 1) {
        readLightsensor();
        line_tracking();
        if (S4 == 1) {
            corner_steering();
            state = 2;
        }
    }
    time = millis();
    while (millis() - time < 1000 && state == 2) {
        readLightsensor();
        line_tracking();
    }
    while (state == 2) {
        readLightsensor();
        line_tracking();
        if (S1 == 1 || S4 == 1) {
            S4 = 0;
            corner_steering();
            state = 3;
        }
    }
    time = millis();
    while (millis() - time < 1000 && state == 3) {
        readLightsensor();
        line_tracking();
    }
    while (state == 3) {
        readLightsensor();
        line_tracking();
        if (S4 == 1) {
            corner_steering();
            state = 4;
        }
    }
    time = millis();
    while (millis() - time < 4500 && state == 4) {
        readLightsensor();
        line_tracking();
    }
    while (state == 4) {
        readLightsensor();
        line_tracking();
        if (S1 == 1 || S4 == 1) {
          S4 = 0 ;
          time = millis();
          while(millis() - time < 100){};
            corner_steering();
            state = 5;
        }
    }
    while (state == 5 && object_being_checked == false) 
    {
        readLightsensor();
        line_tracking();
        ultrasonic_detector(true);
        object_avoidance();
    }
     while (state == 5)
    {
    readLightsensor();
        if (S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0) {
            motor1.run(FORWARD);
            motor1.setSpeed(0);
            motor2.run(FORWARD);
            motor2.setSpeed(0);
            motor3.run(FORWARD);
            motor3.setSpeed(0);
            motor4.run(FORWARD);
            motor4.setSpeed(0);
            state = 100;
        }
        if (state == 100) {
            break;
        } else {
            line_tracking();
        }
    }
}
void ultrasonic_detector(bool run)
{
    digitalWrite(trigger,0); 
    checking_time = micros();
    while (checking_time - micros() < 2)
    {
      if (run == true)
      
      {
        readLightsensor();
        line_tracking();
      }
    }
    digitalWrite(trigger,1); 
    checking_time = micros();
    while (checking_time - micros() < 10)
    {
      if (run == true)
      
      {
        readLightsensor();
        line_tracking();
      }
    } 
    digitalWrite(trigger,0);
    echo_time = pulseIn (echo, HIGH);
    distance = int (echo_time / 2 / 29.412); 
  Serial.print("Khoảng cách: ");
  Serial.print(distance);
  Serial.println(" cm");
  checking_time = millis();
  while (millis() - checking_time < 10)
  {
    if (run == true)
      
      {
        readLightsensor();
        line_tracking();
      }
  }
}

void object_avoidance()
{
  if (distance < 20)
  {
    while(distance > 15) 
    {
      speed = 120;
      ultrasonic_detector(true);
      readLightsensor();
      line_tracking();
    }
    stop();
    checking_time = millis();
    while (millis() - checking_time < 900){}
    servo.write(0);
    checking_time = millis();
    while (millis() - checking_time < 500){}
    for (pos = 1; pos <= 10; pos++)
    {
      servo.write(pos);
      ultrasonic_detector(false);
      if (distance < 30)
      {
        Serial.println("Object is on the right");
        time = millis();

        while (millis() - time < 310)
        {
          strong_left_steering(255, 255);
        }

        time = millis();
        while (millis() - time < 400)
        {
          go_straight(255, 255);
        }

        time = millis();
        while (millis() - time < 360)
        {
          strong_right_steering(255, 255);
        }

        time = millis();
        while (millis() - time < 450)
        {
          go_straight(255, 255);
        }

        time = millis();
        while (millis() - time < 350)
        {
          strong_right_steering(255, 255);
        }

        time = millis();
        while (millis() - time < 450)
        {
          go_straight(255, 255);
        }

        time = millis();
        while (millis() - time < 350)
        {
          strong_left_steering(255, 255);
        }
        object_being_checked = true;
        break;
      }
      checking_time = millis();
      while (millis() - checking_time < 10){};
    }
    if (object_being_checked == false)
    {
      Serial.println("Object is on the left");
      time = millis();

        while (millis() - time < 310)
        {
          strong_right_steering(255, 255);
        }

        time = millis();
        while (millis() - time < 400)
        {
          go_straight(255, 255);
        }

        time = millis();
        while (millis() - time < 360)
        {
          strong_left_steering(255, 255);
        }
        time = millis();
        while (millis() - time < 450)
        {
          go_straight(255, 255);
        }

        time = millis();
        while (millis() - time < 350)
        {
          strong_left_steering(255, 255);
        }

        time = millis();
        while (millis() - time < 450)
        {
          go_straight(255, 255);
        }

        time = millis();
        while (millis() - time < 350)
        {
          strong_right_steering(255, 255);
        }
    }
    speed = 255;
    object_being_checked = true;
  }
}
void setup() {
    pinMode(left, INPUT);
    pinMode(left_center, INPUT);
    pinMode(right_center, INPUT);
    pinMode(right, INPUT);
    time = millis();
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    servo.attach(servoPin);
    servo.write(90);
    Serial.begin(9600);
}

void loop() {
    control();
    Serial.println(S1);
    Serial.println(S2);
    Serial.println(S3);
    Serial.println(S4);
}