#include <AFMotor.h>
#include <Servo.h>
// Định nghĩa các chân và biến
#define left A3// A3  
#define left_center A2// A2
#define right_center A1 //A1
#define right A0 //A0
//#define echo A5
#define trigger A4
#define echo A5
short checking = 0;
float Kp = 30;
uint8_t multiP = 2;
double steeringvalue;
int position, P, previousError, error;
int lspeed, rspeed;
float speed = 255; 
unsigned short steering_time = 600;
unsigned int time;
// unsigned int buzzer_time = 0;
short state = 1;
short S1, S2, S3, S4, S1_previous, S4_previous;
AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
short servoPin = 9;
Servo servo;
bool object_being_checked = false; // góc
int pos = 0;    // biến pos dùng để lưu tọa độ các Servo
unsigned int echo_time;
unsigned int checking_time = 0; //Thời gian là kiểu số nguyên
int object_location = 0; // bên nào có tổng khoảng cách từ cảm biến siêu âm đến vật thể lớn hơn thì bên đó không có vật thể, object_directon = tổng khoảng cách bên trái - tổng khoảng cách bên phải, object_direction > 0 thì bên phải không có vật thể và ngược lại
int distance = 0; //Khoảng cách là kiểu số nguyên
/*int Ultrasonic_read() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW); // Đảm bảo tắt trigger sau khi sử dụng
  distance = pulseIn(echo, HIGH);
  return distance / 29 / 2; // Tính khoảng cách
}*/

void readlightsensor()
{
  S1_previous = S1;
  S4_previous = S4;
    S1 = analogRead(left) > 350 ? 1 : 0;
    S2 = analogRead(left_center) > 350 ? 1 : 0;
    S3 = analogRead(right_center) > 350 ? 1 : 0;
    S4 = analogRead(right) > 350 ? 1 : 0;
}

void line_tracking() {
  // Tiến nếu cả hai cảm biến trung tâm phát hiện đường
  if (S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0)
  {
    // michael_jackson(255, 255);
    // time = millis();
    // while (millis() - time < 100)
    // {}
      if(previousError < 0)
      {       //Turn left if the line was to the left before
        while (S2 == 0 && S3 == 0)
        {
          strong_left_steering(255, 255);
          readlightsensor();
        }
      }
      else
      {
        while (S2 == 0 && S3 == 0)
        {
          strong_right_steering(255, 255);
          readlightsensor();
        }
      }
  }
        position = (10* (0 * S1 + 3 * S2 + 4 * S3 + 7 * S4)) / (S1 + S2 + S3 + S4);
      error = position - 35;
      P = error;
      steeringvalue = (Kp/pow(10,multiP))*P;
      previousError = error;

      lspeed = speed + steeringvalue;
      rspeed = speed - steeringvalue;
      if (rspeed > lspeed)
      {
        lspeed = 0;
        rspeed = speed;
      }
      else if (rspeed < lspeed) 
      {
        lspeed = speed;
        rspeed = 0;
      }
        go_straight(rspeed, lspeed);
 }

void corner_steering()
{
  time = millis();
  if (S1 == 1)
  {
    while (millis() - time < steering_time)
    {
      strong_left_steering(200, 200);
    }
    //s1_s4 = "s1_on_s4_off";
  }
  else if (S4 == 1)
  {
    while (millis() - time < steering_time)
    {
      strong_right_steering(200,200);
    }
    //s1_s4 = "s1_off_s4_on";
  }
}
// void onBuzzer()
// {
//   digitalWrite(BUZZER, HIGH);
// }

// void offBuzzer()
// {
//   digitalWrite(BUZZER, LOW);
// }

void stop(){
      motor1.run(FORWARD);
      motor1.setSpeed(0);
      motor2.run(FORWARD);
      motor2.setSpeed(0);
      motor3.run(FORWARD);
      motor3.setSpeed(0);
      motor4.run(BACKWARD);
      motor4.setSpeed(0);
}

void control()
{
  //state 1
  // onBuzzer();
  while (millis() - time < 2000 && state == 1)
  {
    // if(millis() - time >= 250) offBuzzer(); 
    readlightsensor();
    line_tracking();
  }
  while (state == 1)
  {
    readlightsensor();
    line_tracking();
    if (S4 == 1 && S4_previous == 1)
    { 
      time = millis();
      while(millis() - time < 100);
      corner_steering();
      state = 2;
      // while(true) stop();
    }
  }
  //state 2
  // onBuzzer();
  time = millis();
  while (millis() - time < 1000 && state == 2)
  {
    //if(millis() - time >= 250) offBuzzer();
    readlightsensor();
    line_tracking();
  }
  //s1_s4 = "s1_on_s4_on";
  while (state == 2)
  {
    readlightsensor();
    line_tracking();
    if ((S1 == 1 || S4 == 1) && (S1_previous == 1 || S4_previous == 1))
    {
      time = millis();
      while(millis() - time < 100);
      S4 = 0; S1 = 1;
      corner_steering();
      state = 3;
      // while(true){
      //   stop();
      // }
    }
  }
  // state 3
  // onBuzzer();
  time = millis();
  while (millis() - time < 500 && state == 3)
  {
    // if(millis() - time >= 250) offBuzzer(); 
    readlightsensor();
    line_tracking();
  }
  while (state == 3)
  {
    readlightsensor();
    line_tracking();
    if (S4 == 1 && S4_previous == 1)
    {
      time = millis();
      while(millis() - time < 100);
      corner_steering();
      state = 4;
    }   
  }
  //state 4
  // onBuzzer();
  time = millis();
  while (millis() - time < 4500 && state == 4)
  {
    // if(millis() - time >= 250) offBuzzer(); 
    readlightsensor();
    line_tracking();
  }
  while (state == 4)
  {
    readlightsensor();
    line_tracking();
    S4 = 0;
    if ((S1 == 1 || S4 == 1) && (S1_previous == 1 || S4_previous == 1))
    {
      S4 = 0, S1 = 1;
      time = millis();
      while(millis() - time < 50);
      corner_steering();
      state = 5;
    }
  }
  //state 5
  // onBuzzer();

  while (state == 5 && object_being_checked == false)
  {
    ultrasonic_detector(true);
    // if(millis() - time >= 250) offBuzzer(); 
    object_avoidance();
    readlightsensor();
    line_tracking();
  }
  while (state == 5)
  {
    readlightsensor();
    if (S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0)
    {
      motor1.run(FORWARD);
      motor1.setSpeed(0);
      motor2.run(FORWARD);
      motor2.setSpeed(0);
      motor3.run(FORWARD);
      motor3.setSpeed(0);
      motor4.run(BACKWARD);
      motor4.setSpeed(0);
      state = 100;
    }
    if (state == 100)
    {
      break;
    }
    else line_tracking();
  }
  // onBuzzer();
  // delay(1000);
  // offBuzzer();
  // delay(1000);
}
void go_straight(int rspeed, int lspeed)
{
  //right
  motor1.run(FORWARD);
  motor1.setSpeed(rspeed);
  motor2.run(FORWARD);
  motor2.setSpeed(rspeed);

  //left
  motor3.run(FORWARD);
  motor3.setSpeed(lspeed);
  motor4.run(BACKWARD);
  motor4.setSpeed(lspeed);
}
void michael_jackson(int rspeed, int lspeed)
{
  //right
  motor1.run(BACKWARD);
  motor1.setSpeed(rspeed);
  motor2.run(BACKWARD);
  motor2.setSpeed(rspeed);

  //left
  motor3.run(BACKWARD);
  motor3.setSpeed(lspeed);
  motor4.run(FORWARD);
  motor4.setSpeed(lspeed);
}

void strong_left_steering(int rspeed, int lspeed)
{
  //right
  motor1.run(FORWARD);
  motor1.setSpeed(rspeed);
  motor2.run(FORWARD);
  motor2.setSpeed(rspeed);

  //left
  motor3.run(BACKWARD);
  motor3.setSpeed(lspeed);
  motor4.run(FORWARD);
  motor4.setSpeed(lspeed);
}

void strong_right_steering(int rspeed, int lspeed)
{
  //right
  motor1.run(BACKWARD);
  motor1.setSpeed(rspeed);
  motor2.run(BACKWARD);
  motor2.setSpeed(rspeed);

  //left
  motor3.run(FORWARD);
  motor3.setSpeed(lspeed);
  motor4.run(BACKWARD);
  motor4.setSpeed(lspeed);
}

void ultrasonic_detector(bool run)
{
  //Phát xung từ chân trig, có độ rộng là 10ms
    digitalWrite(trigger,0); //Tắt chân trig
    checking_time = micros();
    while (checking_time - micros() < 2)
    {
      if (run == true)
      
      {
        readlightsensor();
        line_tracking();
      }
    }
    digitalWrite(trigger,1); //bật chân trig để phát xung
    checking_time = micros();
    while (checking_time - micros() < 10)
    {
      if (run == true)
      
      {
        readlightsensor();
        line_tracking();
      }
    } //Xung có độ rộng là 10 microsecond
    digitalWrite(trigger,0);
  
  //Chân echo sẽ nhận xung phản xạ lại
  //Và đo độ rộng xung cao ở chân echo
    echo_time = pulseIn (echo, HIGH);
  //Tính khoảng cách đến vật thể (Đơn vị đo là cm)
  //Tốc độ của âm thanh trong không khí là 340 m/s, tương đương 29,412 microSeconds/cm,
  //Do thời gian được tính từ lúc phát tín hiệu tới khi sóng âm phản xạ lại,
  //vì vậy phải chia cho 2
    distance = int (echo_time / 2 / 29.412); 
  //In lên Serial kết quả
  Serial.print("Khoảng cách: ");
  Serial.print(distance);
  Serial.println(" cm");
  /*Serial.print("Góc: ");
  Serial.print(pos);
  Serial.println(" độ");*/
  checking_time = millis();
  while (millis() - checking_time < 10)
  {
    if (run == true)
      
      {
        readlightsensor();
        line_tracking();
      }
  }
}

void setup() {
  // Khai báo loại chân
  pinMode(left_center, INPUT);
  pinMode(right_center, INPUT);
  pinMode(right, INPUT);
  pinMode(left, INPUT);
  pinMode(trigger, OUTPUT); //Chân trig xuất tín hiệu
  pinMode(echo, INPUT); //Chân echo nhận tín hiệu
  servo.attach(servoPin);
  servo.write(90);
  time = millis();
  Serial.begin(9600);
}
void object_avoidance()
{
  if (distance < 30)
  {
    for (int i = 1; i <= 10; i++)
    {
      speed -= 25.5; // speed = 255 - 12.75 * i
      if (speed == 0) 
      {
        rspeed = lspeed = 0;
      }
      checking_time = millis();
      while (millis() - checking_time < 5)
      {
        readlightsensor();
        line_tracking();
      }
    }
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
        // turn left  50 degree
        servo.write(0);
        while (millis() - time < 310)
        {
          strong_left_steering(255, 255);
        }
        //go straight and find the distance between object and 
        time = millis();
        while (millis() - time < 300)
        {
          go_straight(255, 255);
        }
        // turn right 45 degree
        time = millis();
        while (millis() - time < 360)
        {
          strong_right_steering(255, 255);
        }
        //go straight 
        time = millis();
        while (millis() - time < 350)
        {
          go_straight(255, 255);
        }
        // turn right 50 degree
        time = millis();
        while (millis() - time < 350)
        {
          strong_right_steering(255, 255);
        }
        //go straight 
        time = millis();
        while (millis() - time < 350)
        {
          go_straight(255, 255);
        }
        // turn left 50 degree
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
        // turn right 50 degree
        while (millis() - time < 310)
        {
          strong_right_steering(255, 255);
        }
        //go straight 
        time = millis();
        while (millis() - time < 300)
        {
          go_straight(255, 255);
        }
        // turn left 45 degree
        time = millis();
        while (millis() - time < 360)
        {
          strong_left_steering(255, 255);
        }
        //go straight 
        time = millis();
        while (millis() - time < 350)
        {
          go_straight(255, 255);
        }
        // turn left 50 degree
        time = millis();
        while (millis() - time < 350)
        {
          strong_left_steering(255, 255);
        }
        //go straight 
        time = millis();
        while (millis() - time < 350)
        {
          go_straight(255, 255);
        }
        // turn right 50 degree
        time = millis();
        while (millis() - time < 350)
        {
          strong_right_steering(255, 255);
        }
    }
    speed = 255;
    servo.write(90);
    object_being_checked = true;
  }
}
void loop() {
  // readlightsensor();
  // line_tracking();
  // ultrasonic_detector(true);
  // object_avoidance();

  control();
  // readlightsensor();
  // line_tracking();
  // ultrasonic_detector(true);
  // if (distance <= 30)
  // {
  //   stop();
  //   time = millis();
  //   while (millis() - time < 900){}
  //   servo.write(45);
  //   ultrasonic_detector(false);
  //   object_location += distance;
  //   Serial.println(distance);
  //   servo.write(135);
  //   ultrasonic_detector(false);
  //   object_location -= distance;
  //   Serial.println(distance);
  //   delay (5000);
    // if (object_location < 0)
    // {
    //   // turn left 45 degree
    //   time = millis();
    //   while (millis() - time < 250)
    //   {
    //     strong_left_steering(255, 255);
    //   }
    //   //go straight 
    //   time = millis();
    //   while (millis() - time < 250)
    //   {
    //     go_straight(255, 255);
    //   }
    //   // turn right 45 degree
    //   time = millis();
    //   while (millis() - time < 250)
    //   {
    //     strong_right_steering(255, 255);
    //   }
    //   //go straight 
    //   time = millis();
    //   while (millis() - time < 250)
    //   {
    //     go_straight(255, 255);
    //   }
    //   // turn right 45 degree
    //   time = millis();
    //   while (millis() - time < 250)
    //   {
    //     strong_right_steering(255, 255);
    //   }
    //   //go straight 
    //   time = millis();
    //   while (millis() - time < 250)
    //   {
    //     go_straight(255, 255);
    //   }
    // }
  //}
  // Serial.println(distance);
  
  // Serial.println(analogRead(left));
  //  Serial.println(analogRead(left_center));
  //  Serial.println(analogRead(right_center));
  //  Serial.println(analogRead(right));
  //  Serial.println();
  // Serial.println();
  // delay(500);
}
