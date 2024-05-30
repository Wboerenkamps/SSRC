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
void SSRC::stateMachine(){
    switch(state){
        case initialize:
            init();
            state = scrambling;
            break;
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

            if(test_rotations > 5){
                state = computing;
            }
            //go to computing when it is put on a table for 5 seconds
            break;
        case computing:
            Serial.println("computing....");
            algo.solveCube();
            algo.simplifyQueue();
            algo.printQueue();
            // when done go to 
            state = resolving;
            break;
        case resolving:
            Serial.println("resolving....");
            //algo.printQueue();
            resolve();
            state = interrupted;
            break;
        case interrupted:
            break;
        case solved:
            break;
    }
};
void SSRC::scramble(){
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
            algo.rotateFaceClockwise(algo.intToCubeFace(2));
            algo.printCube();
            test_rotations++;
            break;
        case 3:
            algo.rotateFaceCounterClockwise(algo.intToCubeFace(2));
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