#include "encoder.h"
Encoder::Encoder() {}

Encoder::~Encoder() {}
void Encoder::init(int address)
{

  this->address = address;
  delay(20);
  Serial.println("encoder init");
  Serial.print("address: ");
  Serial.println(address);
  checkMagnetPresence(); // check the magnet (blocks until magnet is found)
  readRawAngle();        // make a reading so the degAngle gets updated
  // startAngle = degAngle + 45; //update startAngle with degAngle - for taring
  startAngle = degAngle;
}
float Encoder::readRawAngle()
{
  selectChannel();
  // 7:0 - bits
  lowbyte = i2c.read(0x36, 0x0D);
  highbyte = i2c.read(0x36, 0x0C);
  // 4 bits have to be shifted to its proper place as we want to build a 12-bit number
  highbyte = highbyte << 8; // shifting to left
  // Finally, we combine (bitwise OR) the two numbers:
  rawAngle = highbyte | lowbyte; // int is 16 bits (as well as the word)
  // Calculate the angle:
  // 12 bit -> 4096 different levels: 360° is divided into 4096 equal parts:
  // 360/4096 = 0.087890625
  // Multiply the output of the encoder with 0.087890625
  degAngle = rawAngle * 0.087890625;
  return degAngle;
}

float Encoder::correctAngle()
{
  // Read raw angle from encoder
  readRawAngle();

  // Recalculate angle relative to startAngle
  correctedAngle = degAngle - startAngle;

  // Normalize negative angles
  if (correctedAngle < 0)
  {
    correctedAngle += 360; // Correction for negative numbers (i.e. -15 becomes +345)
  }

  // Print the corrected/tared angle
  Serial.print("Corrected angle: ");
  Serial.println(correctedAngle, 2);
  // Check if the absolute difference is within the acceptable range

  // Return the corrected angle
  return correctedAngle;
}

void Encoder::selectChannel()
{
  if (address > 7)
    return;
  
  Wire.beginTransmission(MUX_ADDRESS);
  Wire.write(1 << address);
  Wire.endTransmission();
};
void Encoder::checkQuadrant()
{
  previousquadrantNumber = quadrantNumber; // update to the current quadrant
  /*
  //Quadrants:
  4  |  1
  ---|---
  3  |  2
  */

  float quadAngle = correctAngle() + 45;
  if (quadAngle >= 0 && quadAngle <= 90)
  {
    quadrantNumber = 1;
  }

  // Quadrant 2
  if (quadAngle > 90 && quadAngle <= 180)
  {
    quadrantNumber = 2;
  }

  // Quadrant 3
  if (quadAngle > 180 && quadAngle <= 270)
  {
    quadrantNumber = 3;
  }

  // Quadrant 4
  if (quadAngle > 270 && quadAngle < 360)
  {
    quadrantNumber = 4;
  }

  // if(previousquadrantNumber != quadrantNumber){
  //   Serial.print("current quadrant: ");
  // Serial.println(quadrantNumber);
  // Serial.print("previous quadrant: ");
  // Serial.println(previousquadrantNumber);
  // }
  // Serial.print("Quadrant: ");
  // Serial.println(quadrantNumber); //print our position "quadrant-wise"

  // this could be done between every quadrants so one can count every 1/4th of transition

  // Serial.print("Turns: ");
  // Serial.println(numberofTurns,0); //number of turns in absolute terms (can be negative which indicates CCW turns)

  // after we have the corrected angle and the turns, we can calculate the total absolute position
  totalAngle = (numberofTurns * 360) + correctedAngle; // number of turns (+/-) plus the actual angle within the 0-360 range
  // Serial.print("Total angle: ");
  // Serial.print(totalAngle, 2); //absolute position of the motor expressed in degree angles, 2 digits
}

void Encoder::checkMagnetPresence()
{
  selectChannel();
  // This function runs in the setup() and it locks the MCU until the magnet is not positioned properly
  // Serial.println("checking magnet");
  while ((magnetStatus & 32) != 32) // while the magnet is not adjusted to the proper distance - 32: MD = 1
  {
    magnetStatus = 0; // reset reading
    magnetStatus = i2c.read(0x36, 0x0B); //DEFINE MAGIC NUMBERS


    // Wire.beginTransmission(0x36); // connect to the sensor
    // Wire.write(0x0B);             // figure 21 - register map: Status: MD ML MH
    // Wire.endTransmission();       // end transmission
    // Wire.requestFrom(0x36, 1);    // request from the sensor

    // while (Wire.available() == 0)
    //   ;                         // wait until it becomes available
    // magnetStatus = Wire.read(); // Reading the data after the request

    Serial.print("Magnet status: ");
    Serial.println(magnetStatus, BIN); // print it in binary so you can compare it to the table (fig 21)
  }

  // Status register output: 0 0 MD ML MH 0 0 0
  // MH: Too strong magnet - 100111 - DEC: 39
  // ML: Too weak magnet - 10111 - DEC: 23
  // MD: OK magnet - 110111 - DEC: 55

  Serial.println("Magnet found!");
  // delay(1000);
}

int Encoder::getquad()
{
  return quadrantNumber;
}

int Encoder::getPrevQuad()
{
  return previousquadrantNumber;
}
