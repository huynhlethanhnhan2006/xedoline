#include <AFMotor.h>
#define sensorLeft A0
#define sensorMiddle A1
#define sensorRight A2
AF_DCMotor motorLeftForward(1, MOTOR12_KHZ);
AF_DCMotor motorLeftBackward(2, MOTOR12_KHZ);
AF_DCMotor motorRightForward(3, MOTOR12_KHZ);
AF_DCMotor motorRightBackward(4, MOTOR12_KHZ);
void setup() {
    pinMode(sensorLeft, INPUT);
    pinMode(sensorMiddle, INPUT);
    pinMode(sensorRight, INPUT);
    pinMode(soundSensor, INPUT);
  
    pinMode(motorLeftForward, OUTPUT);
    pinMode(motorLeftBackward, OUTPUT);
    pinMode(motorRightForward, OUTPUT);
    pinMode(motorRightBackward, OUTPUT);

    Serial.begin(9600); 

void loop() {
    int leftValue = digitalRead(sensorLeft);
    int middleValue = digitalRead(sensorMiddle);
    int rightValue = digitalRead(sensorRight);
    int soundValue = digitalRead(soundSensor);
    if (middleValue == HIGH) { // Nếu cảm biến giữa phát hiện đường
        moveForward(); // Tiến về phía trước
    } else if (leftValue == HIGH) { // Nếu cảm biến trái phát hiện đường
        turnRight(); // Quay phải
    } else if (rightValue == HIGH) { // Nếu cảm biến phải phát hiện đường
        turnLeft(); // Quay trái
    } else {
        stop(); // Dừng lại nếu không phát hiện đường
    }

    // Kiểm tra âm thanh
    if (soundValue == HIGH) { 
        stop(); // Dừng lại nếu có âm thanh lớn
        delay(1000); // Dừng trong một giây trước khi tiếp tục
    }
}

// Hàm di chuyển về phía trước
void moveForward() {
    digitalWrite(motorLeftForward, HIGH);
    digitalWrite(motorLeftBackward, LOW);
    digitalWrite(motorRightForward, HIGH);
    digitalWrite(motorRightBackward, LOW);
}

// Hàm quay trái
void turnLeft() {
    digitalWrite(motorLeftForward, LOW);
    digitalWrite(motorLeftBackward, LOW);
    digitalWrite(motorRightForward, HIGH);
    digitalWrite(motorRightBackward, LOW);
}

// Hàm quay phải
void turnRight() {
    digitalWrite(motorLeftForward, HIGH);
    digitalWrite(motorLeftBackward, LOW);
    digitalWrite(motorRightForward, LOW);
    digitalWrite(motorRightBackward, LOW);
}

// Hàm dừng lại
void stop() {
    digitalWrite(motorLeftForward, LOW);
    digitalWrite(motorLeftBackward, LOW);
    digitalWrite(motorRightForward, LOW);
    digitalWrite(motorRightBackward, LOW);
}