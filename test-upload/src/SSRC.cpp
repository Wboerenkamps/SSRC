#include "SSRC.h"
#include "string"
SSRC::SSRC(){

}
SSRC::~SSRC(){

}
void SSRC::setState(states newState){

}
int SSRC::getState(){
    return state;
}
void SSRC::testSolve(){
    algo.shuffleCube();
    algo.printCubeInFormat();
    algo.solveCube();
    algo.simplifyQueue();
    algo.printQueue();
}
void SSRC::demo(){
    switch(state){
        case initialize:
            this->init();
            state = idle;
            Serial.print("Init\n");
            break;
        case idle:
            delay(100);

            Serial.print("IDLE\n");
            //Serial.print(accelerometer.idleGet());
            //Serial.println();
            //accelerometer.ReadData();
            //if (!accelerometer.idleGet())
            //    this->state = scrambling;
            //if (accelerometer.solveGet())
            state = scrambling;
            break;
        case scrambling:
            scramble();
            //accelerometer.ReadData();
            //if(accelerometer.idleGet()){
            //    this->state= idle;
            //}
            if(test_rotations > 5){
                state = computing;
            }
            //go to computing when it is put on a table for 5 seconds
            break;
        case computing:
            algo.solveCube();
            algo.simplifyQueue();
            algo.printQueue();
            // when done go to
            state = resolving;
            break;
        case resolving:
            //accelerometer.solveSet(false);
            //accelerometer.scrambleSet(false);
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
void SSRC::stateMachine(){
    switch(state){
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
            if(accelerometer.idleGet()){
                this->state= idle;
            }
            // if(test_rotations > 5){
            //     state = computing;
            // }
            //go to computing when it is put on a table for 5 seconds
            break;
        case computing:
            algo.solveCube();
            // when done go to
            state = resolving;
            break;
        case resolving:
            //#TODO resume function 
            accelerometer.solveSet(false);
            accelerometer.scrambleSet(false);
            resolve();
            state = idle;
            break;
        case interrupted:
            //#TODO pickup implementation
            break;
        case solved:
            //#TODO solved implementation
            break;
    }
};
void SSRC::scramble(){
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
    if(rotation > 0){
        Serial.print("motor: ");
        Serial.print(1);
        Serial.println(" rotation Detected");
        delay(100);
    }
    switch(rotation)
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
    //Serial.print("rotations: ");
    //Serial.println(test_rotations);
    delay(100);
}
void SSRC::resolve(){
    for (const std::string& value : algo.solveQueue) {
        //std::cout << value << ",";
        std::string turnValue = value.c_str();
        
        if (turnValue == "F") { motors[0].moveClockwise(); }
        else if (turnValue == "'F") { motors[0].moveCounterClockwise(); }
        else if (turnValue == "L") { motors[1].moveClockwise(); }
        else if (turnValue == "'L") { motors[1].moveCounterClockwise(); }
        else if (turnValue == "R") { motors[2].moveClockwise(); }
        else if (turnValue == "'R") { motors[2].moveCounterClockwise(); }
        else if (turnValue == "B") { motors[3].moveClockwise(); }
        else if (turnValue == "'B") { motors[3].moveCounterClockwise(); }
        else if (turnValue == "U") { motors[4].moveClockwise(); }
        else if (turnValue == "'U") { motors[4].moveCounterClockwise(); }
        else if (turnValue == "D") { motors[5].moveClockwise(); }
        else if (turnValue == "'D") { motors[5].moveCounterClockwise(); }
            
    }
}
void SSRC::testEncoder(){
    motors[0].testRotate();
    //delay(250);
}
void SSRC::testMotor(){

    delay(2000);
    motors[0].moveClockwise();
    delay(2000);
    motors[0].moveClockwise();
    delay(2000);
    motors[0].moveCounterClockwise();
    delay(2000);
    motors[0].moveClockwise();
    delay(2000);
}
void SSRC::init(){
    // for(int i = 1; i < 6; i++){
    //     motors[i - 1].init(1,i);
    // }
    motors[0].init(1,1);
    
}