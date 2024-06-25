#include "i2cFunction.h"

#define SDA 6
#define SCL 7

i2cFunction::i2cFunction() {}

void i2cFunction::wireInit() 
{
    Wire.begin(SDA,SCL);
}

void i2cFunction::write(uint16_t deviceAddress, uint16_t registerAddress, uint16_t value)
{
    Serial.print("writing address: ");
    Serial.print(deviceAddress);
    Serial.println();
    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.write(value);
    Wire.endTransmission();
    delay(10);
}

byte i2cFunction::read(uint16_t deviceAddress, uint16_t registerAddress)
{
    Serial.print("Read from: ");
    Serial.print(deviceAddress);
    Serial.println();

    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.endTransmission();
    byte requestBytes = Wire.requestFrom(deviceAddress, 1);

    while(Wire.available() == 0); 

    Serial.print("Read bytes: ");
    Serial.print(requestBytes);
    Serial.println();

    return Wire.read();
}