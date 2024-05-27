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
void Motor::rotate(int dir,int speed) {
    byte voltage = speed;
    byte directionByte = 0b00;
    switch (dir) {
        case 1:
            directionByte = 0b01;
            //Serial.println("Forward");
            break;
        case 2:
            directionByte = 0b10;
            //Serial.println("Backwards");
            break;
        case 3:
            directionByte = 0b11;
            //Serial.println("Brake");
            break;
        case 4:
            directionByte = 0b00;
            //Serial.println("Standby");
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
void Motor::testRotate(){
    this->rotate(FORWARD,8);
    encoder.correctAngle();
    delay(20);
}
void Motor::setPosition(int pos,int dir) {
    float targetAngle;
    switch (pos) {
        case 1:
            targetAngle = 0;
            Serial.println("0");
            break;
        case 2:
            targetAngle = 90;
            Serial.println("90");
            break;
        case 3:
            targetAngle = 180;
            Serial.println("180");
            break;
        case 4:
            targetAngle = 270;
            Serial.println("270");
            break;
        default:
            Serial.println("Invalid position");
            return;
    }
    
    moveToAngle(targetAngle, dir);
    Serial.println("Position reached");
}

void Motor::moveToAngle(float targetAngle,int dir) {
    const float THRESHOLD = 5.0; // Define a threshold for the target angle
    const float MAX_SPEED = 56;  // Define maximum motor speed
    const float MIN_SPEED = 6;  // Define minimum motor speed
    const float MAX_INTEGRAL = 100; // Define maximum integral term to prevent windup

    while (true) {
        float angle = encoder.correctAngle();
        float error = targetAngle - angle;
        Serial.print("error: ");
        Serial.println(error);
        // Serial.print("target: ");
        // Serial.println(targetAngle);
        // if (error > 180) {
        //     error -= 360;
        // } else if (error < -180) {
        //     error += 360;
        // }

        // integral += error;

        // Prevent integral windup
        // if (integral > MAX_INTEGRAL) integral = MAX_INTEGRAL;
        // if (integral < -MAX_INTEGRAL) integral = 0;

        // float derivative = error - previousError;
        // float raw_output = Kp * error + Ki * integral + Kd * derivative;

        // // Clamp the raw output to the range -100 to 100
        // if (raw_output > 100) raw_output = 100;
        // if (raw_output < 0) raw_output = 0;

        //Serial.print("output: ");
        //Serial.println(raw_output);

        // Update the previous error
        // previousError = error;

        // // Check if the absolute error is within the threshold to consider the angle reached
        if (abs(error) < THRESHOLD) {
            this->rotate(3, 55); // Assuming 3 and 63 are constants for holding position
            this->rotate(4, 55); // Assuming 3 and 63 are constants for holding position
            
            Serial.println("position reached");
            delay(1000);
            // int starttime = millis();
            // int endtime = starttime;
            // while ((endtime - starttime) <=1000) // do this loop for up to 1000mS
            // {           
            //     float angle = encoder.correctAngle();
            //     delay(20);
            //     endtime = millis();
            // }
            break;
        }
        // Map the raw output to the speed range (MIN_SPEED to MAX_SPEED)
        //int output = mapOutputToSpeed(abs(raw_output), 0, 100, MIN_SPEED, MAX_SPEED);
        delay(20);
        if (dir == 1) {
            this->rotate(FORWARD, 10);
        } else if(dir == 2) {
            this->rotate(BACKWARDS, 10); // Pass positive speed value
        }
        
    }
}

void Motor::moveClockwise(){
    int newPos;
    if(this->position == 4){
        newPos = 1;
    }else{
        newPos = this->position + 1;
    }
    Serial.print("clockwise to: ");
    Serial.println(newPos);
    this->setPosition(newPos,1);
    
    
    position = newPos;
}
void Motor::moveCounterClockwise(){
    int newPos;
    if(position == 1){
        newPos = 4;
    }else{
        newPos = position - 1;
    }
    Serial.print("counter clockwise to: ");
    Serial.println(newPos);
    delay(1000);
    this->setPosition(newPos,2);
    
    
    position = newPos;
}
// Function to map raw PID output to the speed range (6 to 63)
int Motor::mapOutputToSpeed(float raw_output, float raw_min, float raw_max, int speed_min, int speed_max) {
    // Clip raw output to the range [raw_min, raw_max]
    if (raw_output > raw_max) raw_output = raw_max;
    if (raw_output < raw_min) raw_output = raw_min;

    // Perform linear mapping from raw_output range to speed range
    int mapped_speed = (int)((raw_output - raw_min) * (speed_max - speed_min) / (raw_max - raw_min) + speed_min);

    // Clip the mapped speed to the allowable range
    if (mapped_speed > speed_max) mapped_speed = speed_max;
    if (mapped_speed < speed_min) mapped_speed = speed_min;

    return mapped_speed;
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
    this->rotate(3,63);
    encoder.init(encoderAddress);
    
}