#include "motor.h"
#include "cfop.h"
#include "Accelerometer.h"
#include <map>
#define codeLed 6

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

        void stateMachine();
        void testMotor();
        void testEncoder();
        void scramble();
        void resolve();
        void init();
        void demo();
        void testSolve();
    private:
        states state = initialize;
        Motor motors[6];
        Cfop algo;
        int test_rotations = 0;

        Accelerometer accelerometer;
        i2cFunction i2c;
        std::map<int, int> motorToEncoderMap;
        
};