#include "motor.h"
#include "cfop.h"
enum states {
    idle,
    scrambling,
    computing,
    resolving,
    interrupted,
    solved
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
        states state = idle;
        Motor motors[6];
        Cfop algo;
};