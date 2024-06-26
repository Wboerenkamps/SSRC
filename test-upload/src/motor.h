
#define VoltageValue 0x3E
#define MOTOR_ADRESS 0x64

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
        int readDRVFaultRegister();
        void clearDRVFaultRegister();
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
        double Kp = 2;
        double Ki = 0.01;
        double Kd = 0.1;
        byte motorAddress;
        float previousError = 0;
        float integral = 0;
        int correctCount = 0;
        const float THRESHOLD = 2.0; // Define a threshold for the target angle
        const float MAX_SPEED = 56;  // Define maximum motor speed
        const float MIN_SPEED = 7;   // Define minimum motor speed
        int position = 1;
        i2cFunction i2c;
};