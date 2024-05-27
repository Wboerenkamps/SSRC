#include "Accelerometer.h"
#define buzzer 8
#define moveLed 9

Accelerometer::Accelerometer() {}

Accelerometer::~Accelerometer() {}

void Accelerometer::Setup()
{
    pinMode(buzzer, OUTPUT);
    pinMode(moveLed, OUTPUT);
    Serial.begin(115200);
    Wire.begin(SDA, SCL);

    SetupData();

    Loop();
}

void Accelerometer::Loop()
{
    while (true)
    {
        ReadData();
        delay(50);
    }
}

void Accelerometer::SetupData()
{
    // Chip setup
    i2cWrite(ADXL345, POWER_CTL, 8);   // Turn on measure bit
    i2cWrite(ADXL345, DATA_FORMAT, 0); // Set g range to 2g
    i2cWrite(ADXL345, BW_RATE, 6);     // Set bandwidth and output data rate
    callibrate();
}

void Accelerometer::ReadData()
{
    // Serial.println();
    // Serial.print(yMotionThreshHoldMax);
    // Serial.println();
    // Serial.print(yMotionThreshHoldMin);
    // Serial.println();

    byte x0 = i2cRead(ADXL345, xDataReg0);
    byte x1 = i2cRead(ADXL345, xDataReg1);

    float xa = convertAxisData(x0, x1);

    // Serial.print("X = ");
    // Serial.print(xa);
    // Serial.print(" g");
    // Serial.println();

    byte y0 = i2cRead(ADXL345, yDataReg0);
    byte y1 = i2cRead(ADXL345, yDataReg1);

    float ya = convertAxisData(y0, y1);
    // Serial.print("Y = ");
    // Serial.print(ya);
    // Serial.print(" g");
    // Serial.println();

    byte z0 = i2cRead(ADXL345, zDataReg0);
    byte z1 = i2cRead(ADXL345, zDataReg1);

    float za = convertAxisData(z0, z1);
    // Serial.print("Z = ");
    // Serial.print(za);
    // Serial.print(" g");
    // Serial.println();

    if (za >= 2)
    {
        scrambleSet(true);
        tone(buzzer, 100);
    }
    if (xa >= 2.03)
    {
        solveSet(true);
        tone(buzzer, 2000);
    }

    if (ya <= yMotionThreshHoldMax && ya >= yMotionThreshHoldMin && 
    xa <= xMotionThreshHoldMax && xa >= xMotionThreshHoldMin && 
    za <= zMotionThreshHoldMax && za >= zMotionThreshHoldMin)
    {
        idleCount++;
        if (idleCount >= 5)
        {
            idleSet(true);
            digitalWrite(moveLed, HIGH);
        }
    }
    else
    {
        idleSet(false);
        idleCount = 0;
        digitalWrite(moveLed, LOW);
    }

    delay(300);
    noTone(buzzer);
}

void Accelerometer::i2cWrite(uint16_t deviceAddress, uint16_t registerAddress, uint16_t value)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.write(value);
    Wire.endTransmission();
    delay(10);
}

byte Accelerometer::i2cRead(uint16_t deviceAddress, uint16_t registerAddress)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.endTransmission();
    Wire.requestFrom(deviceAddress, 1);
    delay(10);
    return Wire.read();
}

float Accelerometer::convertAxisData(byte value0, byte value1)
{
    value1 = value1 & 0x03;

    uint16_t value = (value1 << 8) + value0;
    int16_t valuef = value;
    if (valuef > 511)
    {
        valuef = valuef - 1024;
    }
    return valuef * 0.004;
}

bool Accelerometer::callibrate()
{
    
    for (int i = 0; i < 100; i++)
    {
        byte x0 = i2cRead(ADXL345, xDataReg0);
        byte x1 = i2cRead(ADXL345, xDataReg1);

        xaTotal += convertAxisData(x0, x1);

        byte y0 = i2cRead(ADXL345, yDataReg0);
        byte y1 = i2cRead(ADXL345, yDataReg1);

        yaTotal += convertAxisData(y0, y1);

        byte z0 = i2cRead(ADXL345, zDataReg0);
        byte z1 = i2cRead(ADXL345, zDataReg1);

        zaTotal += convertAxisData(z0, z1);
        
        delay(10);
    }

    float xMotionThreshHold = xaTotal / 100;
    float yMotionThreshHold = yaTotal / 100;
    float zMotionThreshHold = zaTotal / 100;

    xMotionThreshHoldMin = xMotionThreshHold - 0.05;
    xMotionThreshHoldMax = xMotionThreshHold + 0.05;

    yMotionThreshHoldMin = yMotionThreshHold - 0.05;
    yMotionThreshHoldMax = yMotionThreshHold + 0.05;

    zMotionThreshHoldMin = zMotionThreshHold - 0.05;
    zMotionThreshHoldMax = zMotionThreshHold + 0.05;

    return true;
}

void Accelerometer::idleSet(bool state) {
    idle = state;
}

void Accelerometer::scrambleSet(bool state) {
    scramble = state;
}

void Accelerometer::solveSet(bool state) {
    solve = state;
}

bool Accelerometer::idleGet() {
    return idle;
}

bool Accelerometer::scrambleGet() {
    return scramble;
}

bool Accelerometer::solveGet() {
    return solve;
}