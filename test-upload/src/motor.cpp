#include "motor.h"

Motor::Motor()
{
    motorVoltageMode = VoltageValue;
}

Motor::~Motor() {}

int Motor::getPosition()
{
    return encoder.correctAngle();
}



void Motor::rotate(int dir, int speed)
{
    // Serial.println(encoder.correctAngle());
    byte voltage = speed;
    byte directionByte = 0b00;
    switch (dir)
    {
    case 1:
        directionByte = 0b01;
        // Serial.println("Forward");
        break;
    case 2:
        directionByte = 0b10;
        // Serial.println("Backwards");
        break;
    case 3:
        directionByte = 0b11;
        // Serial.println("Brake");
        break;
    case 4:
        directionByte = 0b00;
        // Serial.println("Standby");
        break;
    default:
        // Handle invalid position
        break;
    }

    byte writeByte = (voltage << 2) | directionByte;
    // Serial.print("write byte:");
    // Serial.println(writeByte, BIN);
    // Serial.print("motor address test: ");
    // Serial.println(motorAddress,HEX);
    i2c.write(this->motorAddress, 0x00, writeByte);

    // Wire.beginTransmission(MOTOR_ADRESS);

    // Wire.write(0x00); // Select control register
    // //byte controlByte = 0b11111101; // Assuming this sets direction and other control bits

    // Wire.write(writeByte);

    // Wire.endTransmission();
}

void Motor::testRotate()
{
    // this->rotate(FORWARD,8);
    encoder.correctAngle();
    delay(20);
}

void Motor::setPosition(int pos)
{
    float targetAngle;
    switch (pos)
    {
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

    moveToAngle(targetAngle);
    Serial.println("Position reached");
}

void Motor::moveToAngle(float targetAngle)
{
    
    integral = 0;

    while (true)
    {
        float angle = encoder.correctAngle();
        Serial.println(angle);
        float error = targetAngle - angle;
        // Serial.print("error: ");
        // Serial.println(error);

        // //Update the previous error
        // previousError = error;
        int speed = PID(targetAngle, angle);
        Serial.print("speed: ");
        Serial.println(speed);
        if (abs(error) < THRESHOLD)
        {
            if(correctCount > 3){
                this->rotate(3, 40);
                this->rotate(4, 40);
                correctCount = 0;
                Serial.println("position reached");
                delay(1000);
                break;
            }else{
                correctCount++;
            }
            
        }else{
            correctCount = 0;
        }
        // Map the raw output to the speed range (MIN_SPEED to MAX_SPEED)
        // int output = mapOutputToSpeed(abs(raw_output), 0, 100, MIN_SPEED, MAX_SPEED);
        delay(10);
        //this->rotate(FORWARD, 15);
        if (speed > 0) {
            this->rotate(BACKWARDS, abs(speed));
        } else if(speed < 0) {
            this->rotate(FORWARD, abs(speed)); // Pass positive speed value
        }
    }
}

float Motor::PID(float targetAngle, float angle)
{
    const float MAX_INTEGRAL = 50; // Define maximum integral term to prevent windup
    float error = targetAngle - angle;
    if (error > 180)
    {
        error -= 360;
    }
    else if (error < -180)
    {
        error += 360;
    }

    integral += error;

    // Prevent integral windup
    if (integral > MAX_INTEGRAL)
        integral = MAX_INTEGRAL;
    if (integral < -MAX_INTEGRAL)
        integral = -MAX_INTEGRAL;

    float derivative = error - previousError;
    float raw_output = Kp * error + Ki * integral + Kd * derivative;
    if (raw_output > MAX_SPEED)
        raw_output = MAX_SPEED;
    else if (raw_output < -MAX_SPEED)
        raw_output = -MAX_SPEED;
    else if (raw_output > -MIN_SPEED && raw_output < MIN_SPEED)
    {
        // Set the raw output to either -8 or 8 based on the error's direction
        if (raw_output >= 0)
            raw_output = MIN_SPEED;
        else
            raw_output = -MIN_SPEED;
    }

    Serial.print("output: ");
    Serial.println(raw_output);

    // Update previousError
    previousError = error;

    return raw_output;
}

void Motor::moveClockwise()
{
    int newPos;

    if (this->position == 4)
    {
        newPos = 1;
    }
    else
    {
        newPos = this->position + 1;
    }

    Serial.print("clockwise to: ");
    Serial.println(newPos);
    this->setPosition(newPos);

    position = newPos;
}

void Motor::moveCounterClockwise()
{
    int newPos;

    if (position == 1)
    {
        newPos = 4;
    }
    else
    {
        newPos = position - 1;
    }

    Serial.print("counter clockwise to: ");
    Serial.println(newPos);
    delay(1000);
    this->setPosition(newPos);

    position = newPos;
}

// Function to map raw PID output to the speed range (6 to 63)
int Motor::mapOutputToSpeed(float raw_output, float raw_min, float raw_max, int speed_min, int speed_max)
{
    // Clip raw output to the range [raw_min, raw_max]
    if (raw_output > raw_max)
        raw_output = raw_max;
    if (raw_output < raw_min)
        raw_output = raw_min;

    // Perform linear mapping from raw_output range to speed range
    int mapped_speed = (int)((raw_output - raw_min) * (speed_max - speed_min) / (raw_max - raw_min) + speed_min);

    // Clip the mapped speed to the allowable range
    if (mapped_speed > speed_max)
        mapped_speed = speed_max;
    if (mapped_speed < speed_min)
        mapped_speed = speed_min;

    return mapped_speed;
}

int Motor::checkRotation()
{
    encoder.checkQuadrant();
    int prevQuad = encoder.getPrevQuad();
    int quad = encoder.getquad();

    if (prevQuad != quad)
    {
        Serial.print("current quadrant: ");
        Serial.println(quad);
        Serial.print("previous quadrant: ");
        Serial.println(prevQuad);
    }

    // check if quadrant changes
    if (quad != prevQuad)
    {
        if ((prevQuad == 4 && quad == 1))
        {
            // 4 --> 1 transition: CW rotation
            Serial.println("CW rotation");
            return 1;
        }
        else if (prevQuad == 1 && quad == 4)
        {
            Serial.println("CCW rotation");
            return 3;
        }
        else if (prevQuad > quad)
        {
            // 1 --> 4 transition: CCW rotation
            Serial.println("CCW rotation");
            return 3;
        }
        else if (prevQuad < quad)
        {
            Serial.println("CW rotation");
            return 1;
        }
    }
    else if (quad == (prevQuad + 2) % 4)
    {
        // Quadrants are exactly 2 apart, indicating a 180-degree rotation
        Serial.println("180 degree rotation");
        return 2;
    }

    return 0; // No rotation detected
}

int Motor::readDRVFaultRegister()
{
    byte status = i2c.read(motorAddress, 0x01);

    Serial.print("Status is: ");
    Serial.println(status, BIN);

    return status;
}

void Motor::clearDRVFaultRegister()
{
    i2c.write(motorAddress, 0x01, 0x80);
}

void Motor::init(int motorAddress, int encoderAddress)
{
    this->motorAddress = motorAddress;
    this->rotate(3, 63);
    
    encoder.init(encoderAddress);
}