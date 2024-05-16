#include "Accelerometer.h"

class motionUI {
    public:
        motionUI();
        ~motionUI();

        void motionSetup();
        void motionLoop();
        
    private:
        Accelerometer accelerometer;
};