#pragma once

#ifndef __sol
#define __sol

#include "sokkelo.h"

//even though the solution can be found when building the maze,
//the point of this project is to test AI, so I will not give the AI any solutions
//and instead I will let it learn by itself :)
class Solution : public Maze
{
public:

	void SolveTheMaze();

	void Initialize();
	void Restart(); 

private:

	const sCell* EvaluateNextMove();
	bool GetPossiblePaths(); //returns false if at a deadend
	const sCell* GuessBestPath();

	std::vector<eDir> vPossiblePaths; //directions, where the AI could advance from the current tile
	std::vector<sCell*> vLikely; //likely to be a pixel that is the path to the solution
	std::vector<sCell*> vCurrentCorridor; //if the scan hits a deadend, mark every cell here as a deadend
	uint32_t iGenerations = 0; //how many times has the AI tried to solve this maze
	sCell* sCurrentTile; 
	sCell* sPreviousTile;
	ImVec2 vEndTile;
	bool bFinished = false;

};

#endif