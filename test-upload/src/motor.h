#define WRITE_ADDRESS 0xC0
#define READ_ADDRESS 0xC1
#define VoltageValue 0x3E
#define MOTOR_ADRESS 0x60
#include <Arduino.h>
#include <Wire.h> 
#include "encoder.h"
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
        int PID();
        void init(int motorAddress, int encoderAddress);
    private:
        int motorVoltageMode;
        Encoder encoder;
        int motorAddress;
        int encoderAddress;
};