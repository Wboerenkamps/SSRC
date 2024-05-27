#include "SSRC.h"
SSRC::SSRC(){

}
SSRC::~SSRC(){

}
void SSRC::setState(states newState){

}
int SSRC::getState(){
    return state;
}
void SSRC::stateMachine(){
    switch(state){
        case idle:
            //switch to scamble when the atleast 1 motor is turned in a direction
            for(int i = 0; i < 6;i++){
                int rotation = motors[i].checkRotation();
                switch(rotation){
                    case 0:
                        break;
                    case 1:
                        algo.rotateFaceClockwise(algo.intToCubeFace(i));
                        state = scrambling;
                    case 2:
                        algo.rotateFaceClockwise(algo.intToCubeFace(i));
                        algo.rotateFaceClockwise(algo.intToCubeFace(i));
                        state = scrambling;
                    case 3:
                        algo.rotateFaceClockwise(algo.intToCubeFace(i));
                        state = scrambling;
                }
                if(rotation == 1){
                    algo.rotateFaceClockwise(algo.intToCubeFace(i));
                }else if(rotation == 2){
                    algo.rotateFaceClockwise(algo.intToCubeFace(i));
                } 
            }
            break;
        case scrambling:
            scramble();
            //go to computing when it is put on a table for 5 seconds
            break;
        case computing:
            algo.solveCube();
            // when done go to 
            break;
        case resolving:

        case interrupted:
            break;
        case solved:
            break;
    }
};
void SSRC::scramble(){
    // for(int i = 1; i < 1;i++){
    //             int rotation = motors[i].checkRotation();
    //             Serial.print("motor: ");
    //             Serial.print(i);
    //             Serial.println(" rotation Detected");
    //             switch(rotation){
    //                 case 0:
    //                     break;
    //                 case 1:
    //                     algo.rotateFaceClockwise(algo.intToCubeFace(i));
    //                 case 2:
    //                     algo.rotateFaceClockwise(algo.intToCubeFace(i));
    //                     algo.rotateFaceClockwise(algo.intToCubeFace(i));
    //                 case 3:
    //                     algo.rotateFaceClockwise(algo.intToCubeFace(i));
    //             }
    //             if(rotation == 1){
    //                 algo.rotateFaceClockwise(algo.intToCubeFace(i));
    //             }else if(rotation == 2){
    //                 algo.rotateFaceClockwise(algo.intToCubeFace(i));
    //             } 
    //         }
    //         delay(250);
    int rotation = motors[0].checkRotation();
    if(rotation > 0){
        Serial.print("motor: ");
        Serial.print(1);
        Serial.println(" rotation Detected");
        delay(500);
    }
    switch(rotation)
    {
        case 0:
            break;
        case 1:
            algo.rotateFaceClockwise(algo.intToCubeFace(1));
        case 2:
            algo.rotateFaceClockwise(algo.intToCubeFace(1));
            algo.rotateFaceClockwise(algo.intToCubeFace(1));
        case 3:
            algo.rotateFaceCounterClockwise(algo.intToCubeFace(1));
    }
    //algo.printCube();
    delay(100);
}
void SSRC::resolve(){
}
void SSRC::testEncoder(){
    motors[0].testRotate();
    //delay(250);
}
void SSRC::testMotor(){
    motors[0].moveClockwise();
    //delay(2000);
    // delay(2000);
    // motors[0].moveClockwise();
    // //delay(2000);
    // delay(2000);
    // motors[0].moveCounterClockwise();
    // delay(2000);
    // // delay(10);
    // motors[0].moveClockwise();
    // delay(2000);
    //delay(2000);
}
void SSRC::init(){
    // for(int i = 1; i < 6; i++){
    //     motors[i - 1].init(1,i);
    // }
    motors[0].init(1,1);
    
}