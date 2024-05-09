#include "cfop.h"
#include <Arduino.h>
Cfop::Cfop()
{
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				cube[i][j][k] = solvedCube[i][j][k]; // assume a solved cube is entered
			}
		}
	}
}

Cfop::Cfop(std::string _cube[6][3][3])
{
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				cube[i][j][k] = _cube[i][j][k];
			}
		}
	}
}

Cfop::~Cfop()
{
}

void Cfop::setCube(std::string (&_cube)[6][3][3]) {
    if (_cube != NULL) {
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 3; ++j) {
                for (int k = 0; k < 3; ++k) {
                    cube[i][j][k] = _cube[i][j][k];
                }
            }
        }
        return;
    }

    std::string testCube[6][3][3] =
    {
        {{ "G1", "Y8", "O7" }, { "W8", "W5", "Y4" }, { "R7", "B6", "W9" }},
        {{ "B3", "B4", "O3" }, { "G6", "O5", "B8" }, { "B9", "G8", "R3" }},
        {{ "B1", "G2", "B7" }, { "W2", "B5", "R2" }, { "Y3", "B2", "W7" }},
        {{ "O9", "W4", "G3" }, { "Y6", "R5", "R6" }, { "G9", "O2", "R9" }},
        {{ "O1", "R8", "Y9" }, { "G4", "G5", "O4" }, { "G7", "R4", "W3" }},
        {{ "R1", "W6", "Y1" }, { "O6", "Y5", "O8" }, { "Y7", "Y2", "W1" }},
    };;

	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				cube[i][j][k] = testCube[i][j][k];
			}
		}
	}
}

// set 1 cubie within the cube
void Cfop::setCubie(std::string cubie, int position[3]) {
	cube[position[0]][position[1]][position[2]] = cubie;
}

void Cfop::shuffleCube() 
{
   // std::cout << std::endl << "Shuffling cube..." << std::endl;
    // start with a solved cube configuration
    std::string cube[6][3][3] =
    { {   { "W1", "W2", "W3" }, { "W4", "W5", "W6" }, { "W7", "W8", "W9" }  },
      {   { "O1", "O2", "O3" }, { "O4", "O5", "O6" }, { "O7", "O8", "O9" }  },
      {   { "B1", "B2", "B3" }, { "B4", "B5", "B6" }, { "B7", "B8", "B9" }  },
      {   { "R1", "R2", "R3" }, { "R4", "R5", "R6" }, { "R7", "R8", "R9" }  },
      {   { "G1", "G2", "G3" }, { "G4", "G5", "G6" }, { "G7", "G8", "G9" }  },
      {   { "Y1", "Y2", "Y3" }, { "Y4", "Y5", "Y6" }, { "Y7", "Y8", "Y9" }  },
    };
    setCube(cube);
  //  srand(time(0));
    //int rotateNum = rand() % 20;
    int rotateNum = 50;
    for (int i = 0; i < rotateNum; i++) {
        int dir = random(0, 1); // decide clockwise or counterclockwise
        if (dir == 0) {
            rotateFaceClockwise(static_cast<CubeFace>(random(0,5)));	// randomly decide which face to turn
        }
        else if (dir == 1) {
            rotateFaceCounterClockwise(static_cast<CubeFace>(random(0, 5))); // randomly decide which face to turn
        }
    }

    //std::cout << "Cube has been shuffled:" << std::endl;
    Serial.println("The Cube has been shuffled:");
}

/*
* fill solveQueue with the next symbols:
* "U" = up
* "D" = down
* "L" = left
* "R" = right
* "F" = front
* "B" = back
* ' indicates counter clockwise
*/

// add 1 move to the queue
void Cfop::addToQueue(const std::string& turn) {
    nrMoves++;
    //std::cout << "Move " << nrMoves << ":" << turn << std::endl;
    solveQueue.push_back(turn);
    if (turn == "F") { rotateFaceClockwise(FRONT); }
    else if (turn == "'F") { rotateFaceCounterClockwise(FRONT); }
    else if (turn == "L") { rotateFaceClockwise(LEFT); }
    else if (turn == "'L") { rotateFaceCounterClockwise(LEFT); }
    else if (turn == "R") { rotateFaceClockwise(RIGHT); }
    else if (turn == "'R") { rotateFaceCounterClockwise(RIGHT); }
    else if (turn == "B") { rotateFaceClockwise(BACK); }
    else if (turn == "'B") { rotateFaceCounterClockwise(BACK); }
    else if (turn == "U") { rotateFaceClockwise(UP); }
    else if (turn == "'U") { rotateFaceCounterClockwise(UP); }
    else if (turn == "D") { rotateFaceClockwise(DOWN); }
    else if (turn == "'D") { rotateFaceCounterClockwise(DOWN); }
    //if (allowPrintCube) { printCube(); }
}

// add a list of moves to the queue
void Cfop::addToQueue(const std::string turn[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        nrMoves++;
        //std::cout << "Move " << nrMoves << ":" << turn[i] << std::endl;
        solveQueue.push_back(turn[i]);

        if (turn[i] == "F") { rotateFaceClockwise(FRONT); }
        else if (turn[i] == "'F") { rotateFaceCounterClockwise(FRONT); }
        else if (turn[i] == "L") { rotateFaceClockwise(LEFT); }
        else if (turn[i] == "'L") { rotateFaceCounterClockwise(LEFT); }
        else if (turn[i] == "R") { rotateFaceClockwise(RIGHT); }
        else if (turn[i] == "'R") { rotateFaceCounterClockwise(RIGHT); }
        else if (turn[i] == "B") { rotateFaceClockwise(BACK); }
        else if (turn[i] == "'B") { rotateFaceCounterClockwise(BACK); }
        else if (turn[i] == "U") { rotateFaceClockwise(UP); }
        else if (turn[i] == "'U") { rotateFaceCounterClockwise(UP); }
        else if (turn[i] == "D") { rotateFaceClockwise(DOWN); }
        else if (turn[i] == "'D") { rotateFaceCounterClockwise(DOWN); }
        //if (allowPrintCube) { printCube(); }
    }
   // delete[] turn;
}

void Cfop::printQueue() {
    if (solveQueue.empty()) {
        //std::cout << "The list is empty." << std::endl;
        Serial.println("The list is empty");
        return;
    }
    for (const std::string& value : solveQueue) {
        //std::cout << value << ",";
        Serial.print(value.c_str());
        Serial.print(",");
    }
   // std::cout << std::endl;
   Serial.println();
}

// simplify the queue, 3 the same consecutive are simplified to 1 move the other way around
void Cfop::simplifyQueue() {
    auto queueItem = solveQueue.begin();
    while (std::distance(queueItem, solveQueue.end()) >= 3) {
        if (*queueItem == *std::next(queueItem) && *queueItem == *std::next(queueItem, 2)) {
            // Remove two elements and insert one movement in the opposite direction
            if (*queueItem == "U") { solveQueue.insert(queueItem, "'U"); }
            if (*queueItem == "'U") { solveQueue.insert(queueItem, "U"); }
            if (*queueItem == "D") { solveQueue.insert(queueItem, "'D"); }
            if (*queueItem == "'D") { solveQueue.insert(queueItem, "D"); }
            if (*queueItem == "L") { solveQueue.insert(queueItem, "'L"); }
            if (*queueItem == "'L") { solveQueue.insert(queueItem, "L"); }
            if (*queueItem == "F") { solveQueue.insert(queueItem, "'F"); }
            if (*queueItem == "'F") { solveQueue.insert(queueItem, "F"); }
            if (*queueItem == "R") { solveQueue.insert(queueItem, "'R"); }
            if (*queueItem == "'R") { solveQueue.insert(queueItem, "R"); }
            if (*queueItem == "B") { solveQueue.insert(queueItem, "'B"); }
            if (*queueItem == "'B") { solveQueue.insert(queueItem, "B"); }
            queueItem = solveQueue.erase(queueItem,  
                std::next(queueItem, 3));
            nrMoves--;
        }
        else {
            ++queueItem;
        }
    }
}

// print the cube in a 3x3 format, each side will be printed individually
void Cfop::printCube() {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
            //    std::cout << cube[i][j][k] << " ";
            Serial.print(cube[i][j][k].c_str());
            Serial.print(" ");
            }
           // std::cout << std::endl;
            Serial.println();
        }
        //std::cout << std::endl;
        Serial.println();
    }
}

// print the cube in format to use in the code
void Cfop::printCubeInFormat() {
    std::cout << "std::string cube[6][3][3] = " << std::endl;
    std::cout << "{" << std::endl;
    for (int i = 0; i < 6; ++i)
    {
        std::cout << "{";
        for (int x = 0; x < 3; ++x)
        {
            std::cout << "{ \"" << cube[i][x][0] << "\", \"" << cube[i][x][1] << "\", \"" << cube[i][x][2] << "\" }";
            if (x == 0 || x == 1) {
                std::cout << ", ";
            }
        }
        std::cout << "},";
        std::cout << std::endl;

    }
    std::cout << "};";
    std::cout << std::endl;
    return;
}

// find a certain cubie within the cube
std::array<int, 3> Cfop::findCubie(std::string cubie) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (cube[i][j][k] == cubie) {
                    return { i, j, k };
                }
            }
        }
    }
    return { 6, 6, 6 };
}

void Cfop::solveCube()
{
	cross();
   // printCube();
   // std::cout << "CROSS PHASE COMPLETED. START FIRST LAYER." << std::endl;
	firstLayer();
  //  printCube();
   // std::cout << "FIRST LAYER PHASE COMPLETED. START SECOND LAYER." << std::endl;
	secondLayer();
   // printCube();
    //std::cout << "F2L PHASE COMPLETED. START OLL." << std::endl;
	oll();
    //printCube();
    //std::cout << "OLL PHASE COMPLETED. START MATCHING YELLOW CROSS COLORS." << std::endl;
    yellowCrossColors();
    //printCube();
    //std::cout << "YELLOW CROSS COLORS COMPLETED. START PLLPART1." << std::endl;
    pllPart1();
    //printCube();
    //std::cout << "PLLPART1 COMPLETED. START PLLPART2." << std::endl;
    pllPart2();
    //printCube();
    //std::cout << "PLLPART2 COMPLETED. CHECKING CUBE..." << std::endl;
    checkCubeSolved();
    printQueue();
}

bool Cfop::checkCubeSolved() {
    bool cubeValid = true;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (cube[i][j][k] != solvedCube[i][j][k]) {
                    cubeValid = false;
                    break;
                }
            }
        }
    }

    if (cubeValid) {
      //  std::cout << "Cube is verified and SOLVED." << std::endl;
      Serial.println("Cube is verified and SOLVED.");
    }
    else {
    //    std::cout << "The program was not able to solve this cube." << std::endl;
     Serial.println("The program was not able to solve this cube.");
    }
    return cubeValid;
}


// make a white cross on the bottom of the cube
void Cfop::cross()
{
    std::string crossCubies[4] = { "W2", "W4", "W6", "W8" }; // cubies to be found during this phase
    for (int i = 0; i < 4; i++) {
        bool correctPlace = false;
        while (!correctPlace) {
            // find position of cubie, check if it is in correct position
            std::array<int, 3> foundPosition = findCubie(crossCubies[i]);
            if (cube[foundPosition[0]][foundPosition[1]][foundPosition[2]] ==
                solvedCube[foundPosition[0]][foundPosition[1]][foundPosition[2]]) {
                correctPlace = true;
                break;
            }

            switch (foundPosition[0]) {
            case 0: // white face
                // on the correct face but not correct place
                if (i == 0) { // first cubie can be turned to correct position on white face
                    addToQueue("D"); // keep turning 'down' face clockwise until correct place found
                    break;
                }

                // turn the cubie to the top side
                if (foundPosition[2] == 0) {
                    std::string turns[] = { "L", "L" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 1 && foundPosition[1] == 2) {
                    //turn back side twice, makes sure that the cubie is on the top side
                    std::string turns[] = { "B", "B" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 2) {
                    //turn right side twice, makes sure that the cubie is on the top side
                    std::string turns[] = { "R", "R" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 1:// left face, turn to upper side
                if (foundPosition[2] == 1 && foundPosition[1] == 0) {
                    std::string turns[] = { "L", "F", "'U", "'F", "'L"};
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 1 && foundPosition[1] == 2) {
                    std::string turns[] = { "'L", "F", "'U", "'F", "'L" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 0 && foundPosition[1] == 1) {
                    std::string turns[] = { "'B", "'U", "B" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 2 && foundPosition[1] == 1) {
                    std::string turns[] = { "F", "'U", "'F"};
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 2:// front face, turn to upper side
                if (foundPosition[2] == 1 && foundPosition[1] == 0) {
                    std::string turns[] = { "F", "R", "'U", "'R", "'F" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 1 && foundPosition[1] == 2) {
                    std::string turns[] = { "'F", "R", "'U", "'R", "F" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 0 && foundPosition[1] == 1) {
                    std::string turns[] = { "'L", "'U", "L"};
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 2 && foundPosition[1] == 1) {
                    std::string turns[] = { "R", "'U", "'R" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 3:// right face, turn to upper side
                if (foundPosition[2] == 1 && foundPosition[1] == 0) {
                    std::string turns[] = { "R", "B", "'U", "'B", "'R"};
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 1 && foundPosition[1] == 2) {
                    std::string turns[] = { "'R", "B", "'U", "'B", "R" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 0 && foundPosition[1] == 1) {
                    std::string turns[] = { "'F", "'U", "F"};
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 2 && foundPosition[1] == 1) {
                    std::string turns[] = { "B", "'U", "'B" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 4:// back face, turn to upper side
                if (foundPosition[2] == 1 && foundPosition[1] == 0) {
                    std::string turns[] = { "B", "L", "'U", "'L", "'B"};
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 1 && foundPosition[1] == 2) {
                    std::string turns[] = { "'B", "L", "'U", "'L", "B" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 0 && foundPosition[1] == 1) {
                    std::string turns[] = { "'R", "'U", "R" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[2] == 2 && foundPosition[1] == 1) {
                    std::string turns[] = { "L", "'U", "'L" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 5:
                // it's on the top face, turn to correct position on top face, 
                // then push it in correct position on down face.
                // first check which cubie is currently being put in position
                switch (i) {
                case 0: // cubie W2
                    if (foundPosition[1] != 2) {
                        addToQueue("U");
                    }
                    else { // correct position on top side found, turn to white side
                        std::string turns[] = { "F", "F" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    break;
                case 1: // cubie W4
                    if (foundPosition[2] != 0) {
                        addToQueue("U");
                    }
                    else { // correct position on top side found, turn to white side
                        std::string turns[] = { "L", "L" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    break;
                case 2: // cubie W6
                    if (foundPosition[2] != 2) {
                        addToQueue("U");
                    }
                    else { // correct position on top side found, turn to white side
                        std::string turns[] = { "R", "R" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    break;
                case 3: // cubie W8
                    if (foundPosition[1] != 0) {
                        addToQueue("U");
                    }
                    else { // correct position on top side found, turn to white side
                        std::string turns[] = { "B", "B" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    break;
                default:
                    std::cout << "Error in phase 1 cross:" << std::endl;
                    std::cout << "Unknown cubie" << std::endl;
                    return;
                    break;
                }
                break;
            default:
                std::cout << "Error in phase 1 cross:" << std::endl;
                std::cout << "Unknown side was found" << std::endl;
                return;
                break;
            }

        }
    }
}

// fix the first layer of the cube, the entire down (white) face
// and numbers 7,8,9 for faces left,front,right,back
void Cfop::firstLayer() {
    std::string firstLayerCubies[4] = { "W1", "W3", "W7", "W9" }; // cubies to be found during this phase
    for (int i = 0; i < 4; i++) {
        bool correctPlace = false;
        while (!correctPlace) {
            // find position of cubie
            std::array<int, 3> foundPosition = findCubie(firstLayerCubies[i]);
            // check if the found position is the correct one by comparing it to the solved state of the cube
            if (cube[foundPosition[0]][foundPosition[1]][foundPosition[2]] ==
                solvedCube[foundPosition[0]][foundPosition[1]][foundPosition[2]]) {
                correctPlace = true;
                break;
            }

            switch (foundPosition[0]) {
            case 0: // white face
                // on white face but incorrect corner, get it out, turn to correct place
                if (foundPosition[1] == 0 && foundPosition[2] == 0) { // top left
                    std::string turns[] = { "F", "U", "'F", "'U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[1] == 0 && foundPosition[2] == 2) { // top right
                    std::string turns[] = { "R", "U", "'R", "'U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[1] == 2 && foundPosition[2] == 0) { // bottom left
                    std::string turns[] = { "L", "U", "'L", "'U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[1] == 2 && foundPosition[2] == 2) { // bottom right
                    std::string turns[] = { "B", "U", "'B", "'U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 1:
                if (foundPosition[1] == 0 && foundPosition[2] == 0) { // top left
                    if (i == 2) {
                        std::string turns[] = { "L", "U", "'L", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else { addToQueue("U"); }// turn to another face
                }
                else if (foundPosition[1] == 0 && foundPosition[2] == 2) { // top right
                    if (i == 0) {
                        std::string turns[] = { "F", "U", "'F", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else { addToQueue("U"); }
                }
                else if (foundPosition[1] == 2 && foundPosition[2] == 0) { // bottom left
                    std::string turns[] = { "L", "U", "'L", "'U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[1] == 2 && foundPosition[2] == 2) { // bottom right
                    std::string turns[] = { "F", "U", "'F", "'U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 2:
                if (foundPosition[1] == 0 && foundPosition[2] == 0) { // top left
                    if (i == 0) {
                        std::string turns[] = { "F", "U", "'F", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else { addToQueue("U"); }// turn to another face
                }
                else if (foundPosition[1] == 0 && foundPosition[2] == 2) { // top right
                    if (i == 1) {
                        std::string turns[] = { "R", "U", "'R", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else { addToQueue("U"); }
                }
                else if (foundPosition[1] == 2 && foundPosition[2] == 0) { // bottom left
                    std::string turns[] = { "F", "U", "'F", "'U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[1] == 2 && foundPosition[2] == 2) { // bottom right
                    std::string turns[] = { "R", "U", "'R", "'U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 3: // right face
                if (foundPosition[1] == 0 && foundPosition[2] == 0) { // top left
                    if (i == 1) {
                        std::string turns[] = { "R", "U", "'R", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else { addToQueue("U"); }// turn to another face
                }
                else if (foundPosition[1] == 0 && foundPosition[2] == 2) { // top right
                    if (i == 3) {
                        std::string turns[] = { "B", "U", "'B", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else { addToQueue("U"); }
                }
                else if (foundPosition[1] == 2 && foundPosition[2] == 0) { // bottom left
                    std::string turns[] = { "R", "U", "'R", "'U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[1] == 2 && foundPosition[2] == 2) { // bottom right
                    std::string turns[] = { "B", "U", "'B", "'U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 4:
                if (foundPosition[1] == 0 && foundPosition[2] == 0) { // top left
                    if (i == 3) {
                        std::string turns[] = { "B", "U", "'B", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else { addToQueue("U"); }// turn to another face
                }
                else if (foundPosition[1] == 0 && foundPosition[2] == 2) { // top right
                    if (i == 2) {
                        std::string turns[] = { "L", "U", "'L", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else { addToQueue("U"); }
                }
                else if (foundPosition[1] == 2 && foundPosition[2] == 0) { // bottom left
                    std::string turns[] = { "B", "U", "'B", "'U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[1] == 2 && foundPosition[2] == 2) { // bottom right
                    std::string turns[] = { "L", "U", "'L", "'U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 5:
                if (foundPosition[1] == 0 && foundPosition[2] == 0) { // top left
                    if (i == 2) {
                        std::string turns[] = { "L", "U", "'L", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else { addToQueue("U"); }// turn to another face
                }
                else if (foundPosition[1] == 0 && foundPosition[2] == 2) { // top right
                    if (i == 3) {
                        std::string turns[] = { "B", "U", "'B", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else { addToQueue("U"); }
                }
                else if (foundPosition[1] == 2 && foundPosition[2] == 0) { // bottom left
                    if (i == 0) {
                        std::string turns[] = { "F", "U", "'F", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else { addToQueue("U"); }
                }
                else if (foundPosition[1] == 2 && foundPosition[2] == 2) { // bottom right
                    if (i == 1) {
                        std::string turns[] = { "R", "U", "'R", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else { addToQueue("U"); }
                }
                break;
            default:
                // fault in searching
                //std::cout << "Error in phase First Layer, unknown result in findCubie." << std::endl << std::endl;
                return;
                break;
            }
        }
    }
}

// fix the second layer of the cube
// numbers 4 & 6 for faces left,front,right,back
void Cfop::secondLayer() {
    std::string secondLayerCubies[4] = { "O6", "B6", "R6", "G6" };  // cubies to be found during this phase
    for (int i = 0; i < 4; i++) {
        bool correctPlace = false;
        while (!correctPlace) {
            // find position of cubie
            std::array<int, 3> foundPosition = findCubie(secondLayerCubies[i]);
            // check if the found position is the correct one by comparing it to the solved state of the cube
            if (cube[foundPosition[0]][foundPosition[1]][foundPosition[2]] ==
                solvedCube[foundPosition[0]][foundPosition[1]][foundPosition[2]]) {
                correctPlace = true;
                break;
            }

            // when a cubie is found on a place it shouldn't be, move it to top layer
            // then insert in correct place on second layer
            switch (foundPosition[0]) {
            case 0: // white face
                // error, should not be possible
                // std::cout << "Error in phase Second Layer, cubie is found on white layer." << std::endl;
                return;
                break;
            case 1: // left face
                if (foundPosition[1] == 0) {
                    switch (i) {
                    case 0: {// move into correct position
                        std::string turns[] = { "U", "F", "U", "'F", "'U", "'L", "'U", "L", "U" }; 
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                        break;
                    }
                    case 1: // move to correct position on top face
                        addToQueue("'U");
                        break;
                    case 2:
                        addToQueue("U");
                        addToQueue("U");
                        break;
                    case 3:
                        addToQueue("U");
                        break;
                    default: // unknown cubie found
                        //std::cout << "Error in phase Second Layer, invalid cubie found." << std::endl; 
                        break;
                    }
                }
                else if (foundPosition[1] == 1 && foundPosition[2] == 0) { // move to top face
                    std::string turns[] = { "'U", "'B", "'U", "B", "U", "L", "U", "'L", "'U", };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[1] == 1 && foundPosition[2] == 2) { // move to top face
                    std::string turns[] = { "U", "F", "U", "'F", "'U", "'L", "'U", "L", "U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 2: // front face
                if (foundPosition[1] == 0) {
                    switch (i) {
                    case 0: 
                        addToQueue("U");
                        break;
                    case 1: // move into correct position
                        {
                            std::string turns[] = { "U", "R", "U", "'R", "'U", "'F", "'U", "F", "U" };
                            addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                            break;
                        }
                    case 2:
                        addToQueue("'U");
                        break;
                    case 3:
                        addToQueue("U");
                        addToQueue("U");
                        break;
                    default: // unknown cubie found
                        //std::cout << "Error in phase Second Layer, invalid cubie found." << std::endl;
                        break;
                    }
                }
                else if (foundPosition[1] == 1 && foundPosition[2] == 0) { // move to top face
                    std::string turns[] = { "U", "F", "U", "'F", "'U", "'L", "'U", "L", "U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[1] == 1 && foundPosition[2] == 2) { // move to top face
                    std::string turns[] = { "'U", "R", "U", "'R", "'U", "'F", "'U", "F", "U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 3:
                if (foundPosition[1] == 0) {
                    switch (i) {
                    case 0:
                        addToQueue("U");
                        addToQueue("U");
                        break;
                    case 1:
                        addToQueue("U");
                        break;
                    case 2:
                        {
                            std::string turns[] = { "U", "B", "U", "'B", "'U", "'R", "'U", "R", "U" };
                            addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                            break;
                        }
                    case 3:
                        addToQueue("'U");
                        break;
                    default: // unknown cubie found
                        //std::cout << "Error in phase Second Layer, invalid cubie found." << std::endl;
                        break;
                    }
                }
                else if (foundPosition[1] == 1 && foundPosition[2] == 0) { // move to top face
                    std::string turns[] = { "'U", "'F", "'U", "F", "U", "R", "U", "'R", "'U", };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[1] == 1 && foundPosition[2] == 2) { // move to top face
                    std::string turns[] = { "'U", "'R", "'U", "R", "U", "B", "U", "'B", "'U", };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 4:
                if (foundPosition[1] == 0) {
                    switch (i) {
                    case 0:
                        addToQueue("'U");
                        break;
                    case 1:
                        addToQueue("U");
                        addToQueue("U");
                        break;
                    case 2:
                        addToQueue("U");
                    break;
                    case 3: {
                        std::string turns[] = { "U", "L", "U", "'L", "'U", "'B", "'U", "B", "U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                        break;
                    }
                    default: // unknown cubie found
                        //std::cout << "Error in phase Second Layer, invalid cubie found." << std::endl;
                        break;
                    }
                }
                else if (foundPosition[1] == 1 && foundPosition[2] == 0) { // move to top face
                    std::string turns[] = { "'U", "'R", "'U", "R", "U", "B", "U", "'B", "'U", };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                else if (foundPosition[1] == 1 && foundPosition[2] == 2) { // move to top face
                    std::string turns[] = { "U", "L", "U", "'L", "'U", "'B", "'U", "B", "U" };
                    addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                }
                break;
            case 5:
                if (i == 0) {               // O6
                    if (foundPosition[1] == 2 && foundPosition[2] == 1) {
                        std::string turns[] = { "'U", "'L", "'U", "L", "U", "F", "U", "'F", "'U" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else {
                        addToQueue("U");
                    }
                } else if (i == 1) {        // B6
                    if (foundPosition[1] == 1 && foundPosition[2] == 2) {
                        std::string turns[] = { "'U", "'F", "'U", "F", "U", "R", "U", "'R", "'U", };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else {
                        addToQueue("U");
                    }
                } else if (i == 2) {        // R6
                    if (foundPosition[1] == 0 && foundPosition[2] == 1) {
                        std::string turns[] = { "'U", "'R", "'U", "R", "U", "B", "U", "'B", "'U", };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else {
                        addToQueue("U");
                    }
                } else if (i == 3) {        // G6
                    if (foundPosition[1] == 1 && foundPosition[2] == 0) {
                        std::string turns[] = { "'U", "'B", "'U", "B", "U", "L", "U", "'L", "'U", };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                    else {
                        addToQueue("U");
                    }
                }
                break;
            default:
                //std::cout << "Error in phase Second Layer, unknown result in findCubie." << std::endl << std::endl;
                // fault in searching
                break;
            }
        }
    }
}

// create yellow cross on top face
void Cfop::oll() 
{
    std::string ollCubies[4] = { "Y2", "Y4", "Y6", "Y8" };
    bool yellowCross = false;
    while (!yellowCross) {
        int correctYellow = 0;
        for (int i = 0; i < 4; i++) {
            std::array<int, 3> foundPosition = findCubie(ollCubies[i]);
            if (foundPosition[0] == 5) { // check if yellow cubie is on yellow face
                correctYellow++;
            }
        }
        if (correctYellow == 4) { // check how many cubies are in correct position
            yellowCross = true;
            break;
        }
        // get correct pattern on top of cube
        std::string blueCubie = cube[2][0][1].substr(0, 1);
        std::string redCubie = cube[3][0][1].substr(0, 1);
        std::string greenCubie = cube[4][0][1].substr(0, 1);
        
        if ((correctYellow == 2 && blueCubie == "Y" && redCubie == "Y") ||
            (correctYellow == 2 && blueCubie == "Y" && greenCubie == "Y") ||
            (correctYellow == 0)) {
            std::string turns[] = { "F", "R", "U", "'R", "'U", "'F" };
            addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
        }
        else {
            addToQueue("U");
        }

        if (correctYellow == 1 || correctYellow == 3) {
            // panic
            std::cout << "Unsolvable Cube." << std::endl;
            return;
        }
    }
}

// move the yellow cross colors to match the faces beneath it
void Cfop::yellowCrossColors() {
    std::string yellowCrossCubies[4] = { "O2", "B2", "R2", "G2" };
    bool matchingColors = false;
    while (!matchingColors) {
        int correctColor = 0;
        for (int i = 0; i < 4; i++) {
            std::array<int, 3> foundPosition = findCubie(yellowCrossCubies[i]);
            // check if color is on correct place
            if (cube[foundPosition[0]][foundPosition[1]][foundPosition[2]] ==
                solvedCube[foundPosition[0]][foundPosition[1]][foundPosition[2]]) {
                correctColor++;
            }
        }
        if (correctColor == 4) { // check how many cubies are in correct position
            matchingColors = true;
            break;
        } 
        else if (correctColor == 0 || correctColor == 1) {
            addToQueue("U");
        }
        else if (correctColor == 2 && cube[4][0][1] == "G2" &&
            ( cube[2][0][1] == "B2" || cube[3][0][1] == "R2") ) {
            std::string turns[] = { "R", "U", "'R", "U", "R", "U", "U", "'R"};
            addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
        } 
        else if (correctColor == 2 && cube[3][0][1] == "R2" &&
            (cube[2][0][1] == "B2" || cube[1][0][1] == "O2")) {
            std::string turns[] = { "F", "U", "'F", "U", "F", "U", "U", "'F" };
            addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
        }
        else if (correctColor == 2 && cube[2][0][1] == "B2" && cube[1][0][1] == "O2") {
            std::string turns[] = { "L", "U", "'L", "U", "L", "U", "U", "'L" };
            addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
        }
        else if (correctColor == 2 && cube[1][0][1] == "O2" && cube[4][0][1] == "G2") {
            std::string turns[] = { "B", "U", "'B", "U", "B", "U", "U", "'B" };
            addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
        }
        else if (correctColor == 3) { // check how many cubies are in correct position
            std::cout << "panic, unsolvable cube" << std::endl;
            break;
        }
    }
}

// make sure all corners are on their respective part of the cube
void Cfop::pllPart1()
{
    int unsolvableCounter = 0;
    std::string pllP1Cubies[4] = { "O1", "B1", "R1", "G1" };
    bool cornersInPlace = false;
    while (!cornersInPlace) {
        int correctCorners = 0;
        bool frontCorner = false;
        bool leftCorner = false;
        bool rightCorner = false;
        bool backCorner = false;
        if (cube[1][0][0] == "O1" ||
            cube[1][0][0] == "G3" ||
            cube[1][0][0] == "Y1") {
            correctCorners++;
            backCorner = true;
        }
        if (cube[2][0][0] == "B1" ||
            cube[2][0][0] == "O3" ||
            cube[2][0][0] == "Y7") {
            correctCorners++;
            leftCorner = true;

        }
        if (cube[3][0][0] == "R1" ||
            cube[3][0][0] == "B3" ||
            cube[3][0][0] == "Y9") {
            correctCorners++;
            frontCorner = true;
        }
        if (cube[4][0][0] == "G1" ||
            cube[4][0][0] == "R3" ||
            cube[4][0][0] == "Y3") {
            correctCorners++;
            rightCorner = true;
        }
        // check how many corners are in correct place
        // if not all 4 are in correct position do the corresponding moves
        if (correctCorners == 4) {
            cornersInPlace = true;
            unsolvableCounter = 0;
            break;
        }
        else if (leftCorner && correctCorners != 2 && correctCorners != 4) {
            std::string turns[] = { "U", "F", "'U", "'B", "U", "'F", "'U", "B" };
            addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
        }
        else if (frontCorner && correctCorners != 2 && correctCorners != 4) {
            std::string turns[] = { "U", "R", "'U", "'L", "U", "'R", "'U", "L" };
            addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
        }
        else if (rightCorner && correctCorners != 2 && correctCorners != 4) {
            std::string turns[] = { "U", "B", "'U", "'F", "U", "'B", "'U", "F" };
            addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
        }
        else if (backCorner && correctCorners != 2 && correctCorners != 4) {
            std::string turns[] = { "U", "L", "'U", "'R", "U", "'L", "'U", "R" };
            addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
        }
        else {// if (correctCorners == 0 || correctCorners == 1 || correctCorners == 3) {
            std::string turns[] = { "U", "R", "'U", "'L", "U", "'R", "'U", "L" };
            addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
        }
        if (correctCorners == 2) {
            std::cout << "panic, unsolvable cube" << std::endl;
            return;
        }
    }
}

// after making sure all corners are in the correct position, give them the correct orientation
void Cfop::pllPart2() {
    std::string pllP2Cubies[4] = { "B3", "R3", "G3", "O3" };
    std::string pllP2YellowCubies[4] = { "Y9", "Y3", "Y1", "Y7" };
    for (int i = 0; i < 4; i++) {
        bool cornerCorrect = false;
        while (!cornerCorrect) {
            if (cube[2][0][2] == pllP2Cubies[i] ||
                cube[3][0][0] == pllP2Cubies[i] ||
                cube[5][2][2] == pllP2Cubies[i]) {
                bool cornerOriented = false;
                while (!cornerOriented) {
                    if (cube[5][2][2] == pllP2YellowCubies[i]) {
                        cornerOriented = true;
                    }
                    else {
                        std::string turns[] = { "F", "D", "'F", "'D" };
                        addToQueue(turns, sizeof(turns) / sizeof(turns[0]));
                    }
                }
                cornerCorrect = true;
            }   
            else {
                addToQueue("U");
            }
        }
    }
    std::array<int, 3> posY1 = findCubie("Y1");
    int turnCounter = 0;
    while (posY1[0] != 5 || posY1[1] != 0 || posY1[2] != 0) {
        addToQueue("U");
        turnCounter++;
        if (turnCounter >= 5) {
            return; // cannot be solved
        }
        posY1 = findCubie("Y1");
    }
}

// rotate a give face clockwise, make sure adjacent faces also change in value
void Cfop::rotateFaceClockwise(CubeFace face)
{
    const int ROWS = 3;
    const int COLUMNS = 3;
    const int FACES = 6;
    // Rotate the given face clockwise
    std::string tmp[ROWS][COLUMNS];
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLUMNS; ++col)
        {
            tmp[row][col] = cube[face][COLUMNS - col - 1][row];
        }
    }
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLUMNS; ++col)
        {
            cube[face][row][col] = tmp[row][col];
        }
    }

    // Update the surrounding faces
    std::string tmpRow[COLUMNS];
    switch (face)
    {
    case FRONT:
        // Rotate adjacent rows on FRONT face
        for (int col = 0; col < COLUMNS; ++col)
        {
            tmpRow[col] = cube[UP][2][col]; // store value from upper cube face
            cube[UP][2][col] = cube[LEFT][COLUMNS - col - 1][2]; // put value from left face into top face
            cube[LEFT][COLUMNS - col - 1][2] = cube[DOWN][0][COLUMNS - col - 1];
            cube[DOWN][0][COLUMNS - col - 1] = cube[RIGHT][col][0];
            cube[RIGHT][col][0] = tmpRow[col];
        }
        break;
    case BACK:
        // Rotate adjacent rows on BACK face
        for (int col = 0; col < COLUMNS; ++col)
        {
            tmpRow[col] = cube[UP][0][COLUMNS - col - 1];
            cube[UP][0][COLUMNS - col - 1] = cube[RIGHT][COLUMNS - col - 1][2]; 
            cube[RIGHT][COLUMNS - col - 1][2] = cube[DOWN][2][col];
            cube[DOWN][2][col] = cube[LEFT][col][0];
            cube[LEFT][col][0] = tmpRow[col];
        }
        break;
    case LEFT:
        // Rotate adjacent rows on LEFT face
        for (int row = 0; row < ROWS; ++row)
        {
            tmpRow[row] = cube[UP][ROWS - row - 1][0];
            cube[UP][ROWS - row - 1][0] = cube[BACK][row][COLUMNS - 1];
            cube[BACK][row][COLUMNS - 1] = cube[DOWN][ROWS - row - 1][0];
            cube[DOWN][ROWS - row - 1][0] = cube[FRONT][ROWS - row - 1][0];
            cube[FRONT][ROWS - row - 1][0] = tmpRow[row];
        }
        break;
    case RIGHT:
        // Rotate adjacent rows on RIGHT face
        for (int row = 0; row < ROWS; ++row)
        {
            tmpRow[row] = cube[UP][row][COLUMNS - 1];
            cube[UP][row][COLUMNS - 1] = cube[FRONT][row][COLUMNS - 1];
            cube[FRONT][row][COLUMNS - 1] = cube[DOWN][row][COLUMNS - 1];
            cube[DOWN][row][COLUMNS - 1] = cube[BACK][ROWS - row - 1][0];
            cube[BACK][ROWS - row - 1][0] = tmpRow[row];
        }
        break;
    case UP:
        // Rotate adjacent rows on UP face
        for (int col = 0; col < COLUMNS; ++col)
        {
            tmpRow[col] = cube[LEFT][0][col];
            cube[LEFT][0][col] = cube[FRONT][0][col];
            cube[FRONT][0][col] = cube[RIGHT][0][col];
            cube[RIGHT][0][col] = cube[BACK][0][col];
            cube[BACK][0][col] = tmpRow[col];
        }
        break;
    case DOWN:
        // Rotate adjacent rows on DOWN face
        for (int col = 0; col < COLUMNS; ++col)
        {
            tmpRow[col] = cube[LEFT][ROWS - 1][col];
            cube[LEFT][ROWS - 1][col] = cube[BACK][ROWS - 1][col];
            cube[BACK][ROWS - 1][col] = cube[RIGHT][ROWS - 1][col];
            cube[RIGHT][ROWS - 1][col] = cube[FRONT][ROWS - 1][col];
            cube[FRONT][ROWS - 1][col] = tmpRow[col];
        }
        break;
    }
}

void Cfop::rotateFaceCounterClockwise(CubeFace face)
{
    // Rotate the face three times clockwise to achieve a counterclockwise rotation
    for (int i = 0; i < 3; ++i) {
        rotateFaceClockwise(face);
    }
};
CubeFace Cfop::intToCubeFace(int value){
    return static_cast<CubeFace>(value);
};