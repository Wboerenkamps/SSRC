#include "i2cFunction.h"

#define MUX_ADDRESS 0x70
class Encoder {
    public:
        Encoder();
        virtual ~Encoder();
        float readRawAngle();
        void checkMagnetPresence();
        void checkQuadrant();
        float correctAngle();
        void init(int address);
        int getquad();
        int getPrevQuad();
        
        void selectChannel();
    private:
        int magnetStatus = 0; //value of the status register (MD, ML, MH)
        int address;
        int lowbyte; //raw angle 7:0
        word highbyte; //raw angle 7:0 and 11:8
        int rawAngle; //final raw angle 
        float degAngle = -1; //raw angle in degrees (360/4096 * [value between 0-4095])

        int quadrantNumber = 1, previousquadrantNumber = 1; //quadrant IDs
        float numberofTurns = 0; //number of turns
        float correctedAngle = 0; //tared angle - based on the startup value
        float startAngle = 0; //starting angle
        float prevAngle = -1;
        float totalAngle = 0; //total absolute angular displacement
        float previoustotalAngle = 0; //for the display printing

        i2cFunction i2c;
        
};
