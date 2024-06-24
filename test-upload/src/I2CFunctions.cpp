#include "I2CFunctions.h"

I2CFunctions::I2CFunctions() {}

I2CFunctions::~I2CFunctions() {}

bool I2CFunctions::i2cWrite(uint16_t deviceAddress, uint16_t registerAddress, uint16_t value) {
    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.write(value);
    Wire.endTransmission();
    delay(10);
}

byte I2CFunctions::i2cRead(uint16_t deviceAddress, uint16_t registerAddress) {
    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.endTransmission();
    Wire.requestFrom(deviceAddress, 1);
    delay(10);
    return Wire.read();
}

