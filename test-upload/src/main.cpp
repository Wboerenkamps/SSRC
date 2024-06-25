//#include <Arduino.h>
//#include <Wire.h> //This is for i2C
// #include "encoder.h"
//#include "motor.h"
#include "SSRC.h"

#define SDA 6
#define SCL 7
#define codeLed 6
// Encoder encoder;
//Motor motor;
SSRC cube;
void setup()
{
    Serial.begin(115200);
    Wire.begin(SDA,SCL);
    delay(5000);
    Serial.println("start");
    //pinMode(codeLed, OUTPUT);

    // Serial.begin(115200); //start serial - tip: don't use serial if you don't need it (speed considerations)
    // Wire.begin(SDA,SCL); //start i2C  
    Wire.setClock(400000); //fast clock
    // sleep(5);
    //motor.readStatus();
    // encoder.init();
    //motor.init(1,1);
    //cube.init();
    //motor.rotate(3,6);
    //delay(5000);
    //Serial.print("init start\n");
    cube.init();
    //cube.testSolve();
    delay(5000);
    
}

void loop()
{  
    // Accelerometer accel;
    // accel.Setup();

    Serial.print("Loop start\n");

    //cube.testMotor();
    //digitalWrite(codeLed, HIGH);
   
    //digitalWrite(codeLed, LOW);
    //cube.demo();
    //cube.scramble();
    cube.testEncoder();
    //cube.stateMachine();
    //cube.testEncoder();
    //cube.scramble();
    // motor.setPosition(1);
    // motor.readStatus();
    // delay(2000);
    // motor.clearFault();
    // motor.setPosition(4);
    // motor.readStatus();
    // delay(2000);
    // motor.setPosition(2);
    // motor.readStatus();
    
    // delay(2000);
    // motor.setPosition(4);
    // motor.readStatus();
    // delay(2000);
    //cube.scramble();
    //cube.testMotor();
    //cube.testEncoder();

}