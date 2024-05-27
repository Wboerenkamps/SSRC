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
        void scramble();
        int resolve();
        void init();
    private:
        states state = initialize;
        Motor motors[6];
        Cfop algo;

        Accelerometer accelerometer;
        
};