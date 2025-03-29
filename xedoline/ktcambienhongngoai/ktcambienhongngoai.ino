#include <AFMotor.h>

#define left A3
#define middle1 A2
#define middle2 A1
#define right A0

void setup() {
  
  pinMode(left, INPUT);
  pinMode(middle1, INPUT);
  pinMode(middle2, INPUT);
  pinMode(right, INPUT);

  Serial.begin(9600);
}

void loop() {

  int leftValue = analogRead(left);
  int middle1Value = analogRead(middle1);
  int middle2Value = analogRead(middle2);
  int rightValue = analogRead(right);

  Serial.print("Left: ");
  Serial.print(leftValue);
  Serial.print(" Middle1: ");
  Serial.print(middle1Value);
  Serial.print(" Middle2: ");
  Serial.print(middle2Value);
  Serial.print(" Right: ");
  Serial.println(rightValue);

}