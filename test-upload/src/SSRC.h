#include "motor.h"
#include "cfop.h"
#include "Accelerometer.h"

enum states {
    idle,
    scrambling,
    computing,
    resolving,
    interrupted,
    solved,
    initialize
};
class SSRC{
    public:
        SSRC();
        virtual ~SSRC();
        void setState(states);
        int getState();
        void stateMachine();
        void testMotor();
        void testEncoder();
        void scramble();
        void resolve();
        void init();
    private:
        states state = initialize;
        Motor motors[6];
        Cfop algo;
        int test_rotations = 0;

        Accelerometer accelerometer;
        
};