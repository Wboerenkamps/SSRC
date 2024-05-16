#include "Accelerometer.h"
#define buzzer 8
#define moveLed 9
int l = 0;
Accelerometer::Accelerometer() {}

Accelerometer::~Accelerometer() {}

void Accelerometer::Setup() {
    pinMode(buzzer, OUTPUT);
    pinMode(moveLed, OUTPUT);
    Serial.begin(115200); // Initiate serial communication for printing the results on the Serial monitor
    Wire.begin(SDA, SCL); // Initiate the Wire library

     Wire.beginTransmission(ADXL345);
    Wire.write(0x31);
    Wire.write(0x0);
    Wire.endTransmission();

     Wire.beginTransmission(ADXL345);
    Wire.write(0x2C);
    Wire.write(0x6);
    Wire.endTransmission();

    //i2cWrite(ADXL345, DATA_FORMAT, 0x0);

    //i2cWrite(ADXL345, BW_RATE, 0x6);

    delay(10);
    SetupData();

    loop();
}

void Accelerometer::Loop() {
    while(true) {
        ReadData();
        //readDoubletap();
        delay(50);
    }
}

void Accelerometer::SetupData() {
    // Set ADXL345 in measuring mode
    Wire.beginTransmission(ADXL345); // Start communicating with the device 
    Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
    // Enable measurement
    Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
    Wire.endTransmission();
    delay(10);
}

void Accelerometer::ReadData() {
    Wire.beginTransmission(0x53);
    Wire.write(0x32);
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte x0 = Wire.read();

    Wire.beginTransmission(0x53);
    Wire.write(0x33);
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte x1 = Wire.read();
    x1 = x1 & 0x03;

    uint16_t x = (x1 << 8) + x0;
    int16_t xf = x;
    if (xf > 511) {
        xf = xf - 1024;
    }
    float xa = xf * 0.004;
    Serial.print("X = ");
    Serial.print(xa);
    Serial.print(" g");
    Serial.println();

    Wire.beginTransmission(0x53);
    Wire.write(0x34);
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte y0 = Wire.read();

    Wire.beginTransmission(0x53);
    Wire.write(0x35);
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte y1 = Wire.read();
    y1 = y1 & 0x03;

    uint16_t y = (y1 << 8) + y0;
    int16_t yf = y;
    if (yf > 511) {
        yf = yf - 1024;
    }
    float ya = yf * 0.004;
    Serial.print("Y = ");
    Serial.print(ya);
    Serial.print(" g");
    Serial.println();

    Wire.beginTransmission(0x53);
    Wire.write(0x36);
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte z0 = Wire.read();

    Wire.beginTransmission(0x53);
    Wire.write(0x37);
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte z1 = Wire.read();
    z1 = z1 & 0x03;

    uint16_t z = (z1 << 8) + z0;
    int16_t zf = z;
    if (zf > 511) {
        zf = zf - 1024;
    }
    float za = zf * 0.004;
    Serial.print("Z = ");
    Serial.print(za);
    Serial.print(" g");
    Serial.println();

    if(za >= 2) {
    tone(buzzer, 100);
    }
    if (xa >= 2.03) 
  {
    tone(buzzer, 2000);
  }

  if (ya <= 0.05 && ya >= -0.05 && xa <= 0.12 && xa >= 0.03 && za <= 1 && za >= 0.90) 
  {
    l++;
    if (l >= 5) {
    digitalWrite(moveLed, HIGH);
    }
    
  } else {
    l = 0;
    digitalWrite(moveLed, LOW);
  }

  delay(300);
  noTone(buzzer);
}

// void Accelerometer::i2cWrite(uint16_t deviceAddress, uint16_t registerAddress, uint16_t value) {
//     Wire.beginTransmission(deviceAddress);
//     Wire.write(registerAddress);
//     Wire.write(value);
//     Wire.endTransmission();

// }

