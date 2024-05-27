#include <Arduino.h>
#include <Wire.h> //This is for i2C
// #include "encoder.h"
//#include "motor.h"
#include "SSRC.h"
#define SDA 6
#define SCL 7
// Encoder encoder;
//Motor motor;
SSRC cube;
void setup()
{
    Serial.begin(115200); //start serial - tip: don't use serial if you don't need it (speed considerations)
    Wire.begin(SDA,SCL); //start i2C  
    Wire.setClock(800000L); //fast clock
    sleep(5);
    Serial.println("start");
    //motor.readStatus();
    // encoder.init();
    //motor.init(1,1);
    //cube.init();
    //motor.rotate(3,6);
    delay(1000);
    cube.init();
}

void loop()
{   
    //cube.scramble();
    cube.testMotor();
    //cube.testEncoder();

}