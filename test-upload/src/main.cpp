//#include <Arduino.h>
//#include <Wire.h> //This is for i2C
// #include "encoder.h"
#include "motor.h"
//#include "SSRC.h"

#define SDA 6
#define SCL 7
// Encoder encoder;
Motor motor;
//SSRC cube;
void setup()
{
    Serial.begin(115200);
    Wire.begin(SDA,SCL);
    Serial.println("start");

    // Serial.begin(115200); //start serial - tip: don't use serial if you don't need it (speed considerations)
    // Wire.begin(SDA,SCL); //start i2C  
    Wire.setClock(800000L); //fast clock
    // sleep(5);
    //motor.readStatus();
    // encoder.init();
    motor.init(1,1);
    //cube.init();
    //motor.rotate(3,6);
    delay(5000);
    //cube.init();
}

void loop()
{    
    //cube.demo();
    //cube.testEncoder();
    //cube.stateMachine();
    //cube.testEncoder();
    //cube.scramble();
    Serial.println("cycle start");
    motor.rotate(1,40);
    motor.readStatus();
    delay(2000);
    motor.rotate(3,40);
    motor.readStatus();
    delay(2000);
    motor.rotate(2,40);
    motor.readStatus();
    delay(2000);
    motor.rotate(3,40);
    motor.readStatus();
    delay(2000);
    Serial.println("cycle done");
    //cube.scramble();
    //cube.testMotor();
    //cube.testEncoder();

}