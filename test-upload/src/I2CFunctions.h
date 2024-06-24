#include <Arduino.h> 
#include <Wire.h>
#define MUX_ADDRESS 0x70
class I2CFunctions {
    public:
        I2CFunctions();
        ~I2CFunctions();

        bool i2cWrite(uint16_t deviceAddress, uint16_t reigsterAddress, uint16_t value);
        byte i2cRead(uint16_t deviceAddress, uint16_t registerAddress);
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

};
