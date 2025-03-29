#include <Servo.h> 
int servo = 9;
int goc;

Servo myServo;


void setup() 
{ 
    Serial.begin(9600);
    
    myServo.attach(servo); 
}

 
void loop() 
{ 
    myServo.write(0);
    goc = myServo.read();
    Serial.print("Góc hiện tại: "); Serial.println(goc);
    delay(1000);
  
    myServo.write(90);
    goc = myServo.read();
    Serial.print("Góc hiện tại: "); Serial.println(goc);
    delay(1000);
  
    myServo.write(180);
    goc = myServo.read();
    Serial.print("Góc hiện tại: "); Serial.println(goc);
    delay(1000);

}
