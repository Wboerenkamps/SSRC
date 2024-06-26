#include "SSRC.h"


SSRC cube;
i2cFunction i2c;
Accelerometer accel;

void setup()
{
    Serial.begin(115200);
    i2c.wireInit();
    Wire.begin(SDA,SCL);

  
    //cube.init();
    delay(5000);
    
    Serial.print("Setup complete\n");

    // Wire.setClock(400000); not sure if this is needed
}

void loop()
{  
    accel.Setup();


    // Serial.print("Start of loop\n");
    //cube.testMotor();
    //cube.testEncoder();
    delay(1000);
  
    //digitalWrite(codeLed, HIGH);
   
    //digitalWrite(codeLed, LOW);
    //cube.demo();
    //cube.scramble();
 
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

    // Serial.print("Loop completed\n");
}