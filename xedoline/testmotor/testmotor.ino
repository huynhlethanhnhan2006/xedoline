#include <AFMotor.h>

AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

void testMotor(AF_DCMotor &motor, const char *motorName) {
    Serial.print("Testing ");
    Serial.println(motorName);

    // Chạy tiến
    Serial.println("Running FORWARD...");
    motor.run(FORWARD);
    motor.setSpeed(200);
    delay(2000); // Chạy trong 2 giây

    // Dừng
    Serial.println("Stopping...");
    motor.run(RELEASE);
    delay(1000); // Dừng 1 giây

    // Chạy lùi
    Serial.println("Running BACKWARD...");
    motor.run(BACKWARD);
    motor.setSpeed(200);
    delay(2000); // Chạy trong 2 giây

    // Dừng
    Serial.println("Stopping...");
    motor.run(RELEASE);
    delay(1000); // Dừng 1 giây

    Serial.println("Test complete.\n");
}

void setup() {
    Serial.begin(9600);
    Serial.println("Motor Test Program Starting...");
    delay(2000); // Đợi để khởi động
}

void loop() {
    // Kiểm tra từng động cơ
    testMotor(motor1, "Motor 1");
    testMotor(motor2, "Motor 2");
    testMotor(motor3, "Motor 3");
    testMotor(motor4, "Motor 4");

    // Lặp lại sau một khoảng thời gian
    Serial.println("All motors tested. Restarting in 5 seconds...\n");
    delay(5000);
}
