#include "motor.h"
Motor::Motor(){
    motorVoltageMode = VoltageValue;
}
Motor::~Motor(){

}
int Motor::getPosition(){
    return 1;
}

void Motor::setPosition(int pos){
    
    Wire.beginTransmission(MOTOR_ADRESS);
    Wire.write(0x0);
    byte controlByte = 0b00001001;
    if(pos == 1){
        Wire.write(controlByte);
        Serial.println("forward");
    }else{
        Wire.write(0x3E);
        Serial.println("standby");
    }
    Wire.endTransmission();
    
}
int Motor::checkRotation() {
    encoder.checkQuadrant();
    int prevQuad = encoder.getPrevQuad();
    int quad = encoder.getquad();
        if(prevQuad != quad){
        Serial.print("current quadrant: ");
    Serial.println(quad);
    Serial.print("previous quadrant: ");
    Serial.println(prevQuad);
    }
    if (quad != prevQuad) { // If we changed quadrants
    if ((prevQuad == 4 && quad == 1)) {
        // 4 --> 1 transition: CW rotation
        Serial.println("CW rotation");
        return 1;
    } else if(prevQuad == 1 && quad == 4){
        Serial.println("CCW rotation");
        return 3;
    }else if (prevQuad > quad) {
        // 1 --> 4 transition: CCW rotation
        Serial.println("CCW rotation");
        return 3;
    }else if (prevQuad < quad){
        Serial.println("CW rotation");
        return 1;
    }
} else if (quad == (prevQuad + 2) % 4) {
    // Quadrants are exactly 2 apart, indicating a 180-degree rotation
    Serial.println("180 degree rotation");
    return 2;
}
    
    return 0; // No rotation detected
}

int Motor::readStatus(){
    Wire.beginTransmission(MOTOR_ADRESS);
    Wire.write(0x01);
    Wire.endTransmission();
    Wire.requestFrom(MOTOR_ADRESS,1);
    while(Wire.available() == 0); //wait until it becomes available 
    byte status = Wire.read();
    Serial.println(status);
    return 1;
}
void Motor::init(int motorAddress, int encoderAddress){
    this->motorAddress = motorAddress;
    this->encoderAddress = encoderAddress;
    encoder.init(encoderAddress);
    
}