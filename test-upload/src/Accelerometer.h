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

        void i2cWrite(uint16_t deviceAddress, uint16_t registerAddress, uint16_t value);
        byte i2cRead(uint16_t deviceAddress, uint16_t registerAddress);

        float convertAxisData(byte value0, byte value1);

    private:
        int16_t idleCount = 0;

        int ADXL345 = 0x53;
        int SDA = 6;
        int SCL = 7;
        int buzzer = 8;
        int moveLed = 9;

        uint16_t POWER_CTL = 0x2D;
        uint16_t DATA_FORMAT = 0x31;
        uint16_t BW_RATE = 0x2C;

        // DOUBLE TAP SETUP
        int16_t THRESH_TAP = 0x1D;
        int16_t DUR = 0x21;
        int16_t Latent = 0x22;
        int16_t window = 0x23;
        int16_t tapAxes = 0x2A;

        int16_t INT_SOURCE = 0x30;        

        // DATA REGISTERS
        int16_t xDataReg0 = 0x32;
        int16_t xDataReg1 = 0x33;

        int16_t yDataReg0 = 0x34;
        int16_t yDataReg1 = 0x35;

        int16_t zDataReg0 = 0x36;
        int16_t zDataReg1 = 0x37;
        
};