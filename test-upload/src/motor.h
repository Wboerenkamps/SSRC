#define WRITE_ADDRESS 0xC0
#define READ_ADDRESS 0xC1
#define VoltageValue 0x3E
#define MOTOR_ADRESS 0x64
#include <Arduino.h>
#include <Wire.h> 
#include "encoder.h"
#define FORWARD 1
#define BACKWARDS 2
class Motor 
{
    public:
        Motor();
        virtual ~Motor();
        int getPosition();
        void setPosition(int pos);
        int checkRotation();
        int readStatus();
        void clearFault();
        void testRotate();
        void moveToAngle(float targetAngle);

        float PID(float,float);
        int mapOutputToSpeed(float raw_output, float raw_min, float raw_max, int speed_min, int speed_max);
        void moveClockwise();
        void moveCounterClockwise();
        void rotate(int,int);
        void init(int motorAddress, int encoderAddress);
    private:
        int motorVoltageMode;
        Encoder encoder;
        double Kp = 0.05;
        double Ki = 0.01;
        double Kd = 0.2;
        int motorAddress;
        int encoderAddress;

        // float Kp = 1.0; // Proportional gain
        // float Ki = 0.01; // Integral gain
        // float Kd = 0.1; // Derivative gain
        float previousError = 0;
        float integral = 0;
        int position = 1;
};