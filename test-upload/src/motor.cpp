#include "motor.h"
Motor::Motor(){
    motorVoltageMode = VoltageValue;
}
Motor::~Motor(){

}
int Motor::getPosition(){
    return 1;
}
void Motor::clearFault(){
    Wire.beginTransmission(MOTOR_ADRESS);
    
    Wire.write(0x01); // Select control register
    //byte controlByte = 0b11111101; // Assuming this sets direction and other control bits
    
    Wire.write(0b10000000);
    
    Wire.endTransmission();
}
void Motor::setPosition(int pos) {
    byte voltage = 0x06;
    byte directionByte = 0b00;
    switch (pos) {
        case 1:
            directionByte = 0b01;
            Serial.println("Forward");
            break;
        case 2:
            directionByte = 0b10;
            Serial.println("Backwards");
            break;
        case 3:
            directionByte = 0b11;
            Serial.println("Brake");
            break;
        case 4:
            directionByte = 0b00;
            Serial.println("Standby");
            break;
        default:
            // Handle invalid position
            break;
    }
    
    byte writeByte = (voltage << 2) | directionByte;
    //Serial.print("write byte:");
    //Serial.println(writeByte, BIN);
    
    Wire.beginTransmission(MOTOR_ADRESS);
    
    Wire.write(0x00); // Select control register
    //byte controlByte = 0b11111101; // Assuming this sets direction and other control bits
    
    Wire.write(writeByte);
    
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
    Serial.println(status,BIN);
    return 1;
}
void Motor::init(int motorAddress, int encoderAddress){
    this->motorAddress = motorAddress;
    this->encoderAddress = encoderAddress;
    encoder.init(encoderAddress);
    
}