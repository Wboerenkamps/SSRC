#include "SSRC.h"


SSRC cube;
i2cFunction i2c;

void setup()
{
    Serial.begin(115200);
    i2c.wireInit();
    Wire.begin(SDA,SCL);
    delay(5000);

  
    //cube.init();
    
    Serial.print("Setup complete\n");

    // Wire.setClock(400000); not sure if this is needed
}

void loop()
{  
    // Serial.print("Start of loop\n");
    //cube.fullTest();
    //cube.testEncoder();
    //cube.testMotor();
    cube.testSingleMotor(1,true);
    delay(500);

    // Serial.print("Loop completed\n");
}