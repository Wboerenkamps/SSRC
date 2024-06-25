#include <Arduino.h> 
#include <Wire.h>
#define MUX_ADDRESS 0x70
class i2cFunction {
    public:
        i2cFunction();
        void wireInit();
        void write(uint16_t deviceAddress, uint16_t registerAddress, uint16_t value);
        byte read(uint16_t deviceAddress, uint16_t registerAddress);
    private:


};
