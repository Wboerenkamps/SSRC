#include "SSRC.h"
#include "string"

SSRC::SSRC() {
    motorToEncoderMap[0x60] = 1; // m4 = 00 encoder niet 
    motorToEncoderMap[0x61] = 2; // m6=  0x encoder = 2 -
    motorToEncoderMap[0x63] = 0; // m1 = x0 encoder = 0 -- m/e werkend
    motorToEncoderMap[0x64] = 1; // m3 = xx encoder = 1 -- e werkend
    motorToEncoderMap[0x67] = 1; // m5 = 1x encoder = 1 niet
    motorToEncoderMap[0x68] = 2; // m2 = 11 encoder = 2 niet
}

SSRC::~SSRC() {}

void SSRC::testSolve()
{
    algo.shuffleCube();
    algo.printCubeInFormat();
    algo.solveCube();
    algo.simplifyQueue();
    algo.printQueue();
}

void SSRC::demo()
{
    switch (state)
    {
    case initialize:
        this->init();
        state = idle;
        Serial.print("Init\n");
        break;

    case idle:
        delay(100);
        Serial.print("IDLE\n");
        // Serial.print(accelerometer.idleGet());
        // Serial.println();
        // accelerometer.ReadData();
        // if (!accelerometer.idleGet())
        //     this->state = scrambling;
        // if (accelerometer.solveGet())
        state = scrambling;
        break;

    case scrambling:
        scramble();
        // accelerometer.ReadData();
        // if(accelerometer.idleGet()){
        //     this->state= idle;
        // }
        if (test_rotations > 5)
        {
            state = computing;
        }
        // go to computing when it is put on a table for 5 seconds
        break;

    case computing:
        algo.solveCube();
        algo.simplifyQueue();
        algo.printQueue();
        // when done go to
        state = resolving;
        break;

    case resolving:
        // accelerometer.solveSet(false);
        // accelerometer.scrambleSet(false);
        Serial.println("resolving....");
        resolve();
        state = idle;
        break;

    case interrupted:
        break;

    case solved:
        break;
    }
}
void SSRC::stateMachine()
{
    switch (state)
    {

    case initialize:
        accelerometer.SetupData();
        accelerometer.SetupSleep();
        this->init();
        state = idle;
        Serial.print("Init\n");
        break;

    case idle:
        delay(100);
        Serial.print("IDLE\n");
        Serial.print(accelerometer.idleGet());
        Serial.println();
        accelerometer.ReadData();
        if (!accelerometer.idleGet())
            this->state = scrambling;
        if (accelerometer.solveGet())
            state = resolving;
        break;

    case scrambling:
        scramble();
        accelerometer.ReadData();
        if (accelerometer.idleGet())
        {
            this->state = idle;
        }
        // if(test_rotations > 5){
        //     state = computing;
        // }
        // go to computing when it is put on a table for 5 seconds
        break;

    case computing:
        algo.solveCube();
        // when done go to
        state = resolving;
        break;

    case resolving:
        // #TODO resume function
        accelerometer.solveSet(false);
        accelerometer.scrambleSet(false);
        resolve();
        state = idle;
        break;

    case interrupted:
        // #TODO pickup implementation
        break;

    case solved:
        // #TODO solved implementation
        break;
    }
};
void SSRC::scramble()
{
    // for(int i = 0; i < 6;i++){
    //     int rotation = motors[i].checkRotation();
    //     if(rotation > 0){
    //         Serial.print("motor: ");
    //         Serial.print(1);
    //         Serial.println(" rotation Detected");
    //         delay(100);
    //     }
    //     switch(rotation)
    //     {
    //         case 0:
    //             break;
    //         case 1:
    //             algo.rotateFaceClockwise(algo.intToCubeFace(i));
    //             algo.printCube();
    //             test_rotations++;
    //             break;
    //         case 3:
    //             algo.rotateFaceCounterClockwise(algo.intToCubeFace(i));
    //             algo.printCube();
    //             test_rotations++;
    //             break;
    //     }
    // }
    int rotation = motors[0].checkRotation();
    if (rotation > 0)
    {
        Serial.print("motor: ");
        Serial.print(1);
        Serial.println(" rotation Detected");
        delay(100);
    }
    switch (rotation)
    {
    case 0:
        break;
    case 1:
        algo.rotateFaceClockwise(algo.intToCubeFace(0));
        algo.printCube();
        test_rotations++;
        break;
    case 3:
        algo.rotateFaceCounterClockwise(algo.intToCubeFace(0));
        algo.printCube();
        test_rotations++;
        break;
    }
    // Serial.print("rotations: ");
    // Serial.println(test_rotations);
    delay(100);
}

void SSRC::resolve()
{
    for (const std::string &value : algo.solveQueue)
    {
        std::string turnValue = value.c_str();

        if (turnValue == "F")
        {
            motors[0].moveClockwise();
        }
        else if (turnValue == "'F")
        {
            motors[0].moveCounterClockwise();
        }
        else if (turnValue == "L")
        {
            motors[1].moveClockwise();
        }
        else if (turnValue == "'L")
        {
            motors[1].moveCounterClockwise();
        }
        else if (turnValue == "R")
        {
            motors[2].moveClockwise();
        }
        else if (turnValue == "'R")
        {
            motors[2].moveCounterClockwise();
        }
        else if (turnValue == "B")
        {
            motors[3].moveClockwise();
        }
        else if (turnValue == "'B")
        {
            motors[3].moveCounterClockwise();
        }
        else if (turnValue == "U")
        {
            motors[4].moveClockwise();
        }
        else if (turnValue == "'U")
        {
            motors[4].moveCounterClockwise();
        }
        else if (turnValue == "D")
        {
            motors[5].moveClockwise();
        }
        else if (turnValue == "'D")
        {
            motors[5].moveCounterClockwise();
        }
    }
}
void SSRC::testSingleMotor(int i,bool encoderEnable){
    int index = i;
    int testMotorAddress = 0;
    int testEncoderAddress = 0;
    auto it = motorToEncoderMap.begin();
    std::advance(it, index);

    if (it != motorToEncoderMap.end()) {
        testMotorAddress = it->first;
        testEncoderAddress = it->second;

    } else {
    }
    if(encoderEnable){
        Serial.print("MOTOR ADDRESS TEST FUNCTION: ");
        Serial.println(testMotorAddress,HEX);
        motors[i].init(testMotorAddress,testEncoderAddress);
    }
    delay(500);
    motors[i].rotate(1,40);
    delay(500);
    motors[i].rotate(3,40);
    delay(500);
}
void SSRC::testEncoder()
{
    motors[0].testRotate();
}
void SSRC::fullTest(){
    for(int i = 2; i < 6;i++){
        //motors[i].getPosition();
        motors[i].moveClockwise();
        delay(500);
        motors[i].moveCounterClockwise();
        delay(500);
    }
}
void SSRC::testMotor()
{
    const int testMotor = 3;
    delay(10);
    //i2c.write(MOTOR_ADRESS, 0x01, 0x80);

    motors[testMotor].rotate(1, 40);
    delay(1000);
    motors[testMotor].rotate(3, 40);
    delay(1000);
    motors[testMotor].rotate(2, 40);
    delay(1000);
    motors[testMotor].rotate(3, 40);
    delay(1000);

    // delay(2000);
    // motors[0].moveClockwise();
    // delay(2000);
    // motors[0].moveClockwise();
    // delay(2000);
    // motors[0].moveCounterClockwise();
    // delay(2000);
    // motors[0].moveClockwise();
    // delay(2000);

    //motors[0].readDRVFaultRegister();
}

void SSRC::init()
{
    Serial.print("SSRC init start\n");

    // for(int i = 1; i < 6; i++){
    //     motors[i - 1].init(1,i);
    // }
    int i = 0;
    for (const auto& pair : motorToEncoderMap) {
        if (i > 4){
        Serial.print("init motor address: ");
        Serial.println(pair.first,HEX);
        Serial.print("init encoder address: ");
        Serial.println(pair.second);

        motors[i].init(pair.first,pair.second);
        }
        i++;
    }
    delay(5000);
    //motors[0].init(1, 1);
}  