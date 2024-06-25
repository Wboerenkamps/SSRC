#include "i2cFunction.h"

i2cFunction::i2cFunction() {}

void i2cFunction::write(uint16_t deviceAddress, uint16_t registerAddress, uint16_t value)
{
    Serial.write("writing address: " + deviceAddress);
    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.write(value);
    Wire.endTransmission();
    delay(10);
}

byte i2cFunction::read(uint16_t deviceAddress, uint16_t registerAddress)
{
    Serial.write("reading address: " + deviceAddress);
    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.endTransmission();
    byte request = Wire.requestFrom(deviceAddress, 1);
    while(Wire.available() == 0); 
    Serial.print("request bytes: ");
    Serial.print(request);
    Serial.println();
 
    
    return Wire.read();
}