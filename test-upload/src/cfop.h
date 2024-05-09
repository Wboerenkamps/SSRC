#pragma once

#include <iostream>
#include <string>
#include <list>
#include <array>
enum CubeFace {
		DOWN,
		LEFT,
		FRONT,
		RIGHT,
		BACK,
		UP
	};
class Cfop
{
public:
    
	std::list<std::string> solveQueue;
	int nrMoves = 0;
	const std::string solvedCube[6][3][3] =
	{ {   { "W1", "W2", "W3" }, { "W4", "W5", "W6" }, { "W7", "W8", "W9" }  },
	  {   { "O1", "O2", "O3" }, { "O4", "O5", "O6" }, { "O7", "O8", "O9" }  },
	  {   { "B1", "B2", "B3" }, { "B4", "B5", "B6" }, { "B7", "B8", "B9" }  },
	  {   { "R1", "R2", "R3" }, { "R4", "R5", "R6" }, { "R7", "R8", "R9" }  },
	  {   { "G1", "G2", "G3" }, { "G4", "G5", "G6" }, { "G7", "G8", "G9" }  },
	  {   { "Y1", "Y2", "Y3" }, { "Y4", "Y5", "Y6" }, { "Y7", "Y8", "Y9" }  },
	};

	std::string cube[6][3][3];
	bool allowPrintCube = true;

public:
	Cfop();
	Cfop(std::string _cube[6][3][3]);
	~Cfop();

	void setCube(std::string (&_cube)[6][3][3]);
	void setCubie(std::string cubie, int position[3]);
	void shuffleCube();

	void addToQueue(const std::string& turn);
	void addToQueue(const std::string turn[], size_t size);
	void printQueue();
	void printCube();
	void printCubeInFormat();

	std::array<int, 3> findCubie(std::string cubie);

	void rotateFaceClockwise(CubeFace face);
	void rotateFaceCounterClockwise(CubeFace face);
    CubeFace intToCubeFace(int value);
	void solveCube();
	void cross();
	void firstLayer();
	void secondLayer();
	void oll();
	void yellowCrossColors();
	void pllPart1();
	void pllPart2();

	bool checkCubeSolved();
	void simplifyQueue();
};

