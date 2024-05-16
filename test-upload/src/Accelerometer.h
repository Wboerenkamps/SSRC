#include <cstdint>
#include "Wire.h"
#include <Arduino.h>

class Accelerometer {
    public:
        Accelerometer();
        ~Accelerometer();

        void Setup();
        void Loop();

        void SetupData();
        void ReadData();

       // void i2cWrite(uint16_t deviceAddress, uint16_t registerAddress, uint16_t value);

    private:
        int ADXL345 = 0x53;
        int SDA = 6;
        int SCL = 7;
        int buzzer = 8;
        int moveLed = 9;

        //uint16_t DATA_FORMAT = 0x31;
        //uint16_t BW_RATE = 0x2C;

        // DOUBLE TAP SETUP
        int16_t THRESH_TAP = 0x1D;
        int16_t DUR = 0x21;
        int16_t Latent = 0x22;
        int16_t window = 0x23;
        int16_t tapAxes = 0x2A;

        int16_t INT_SOURCE = 0x30;        
};