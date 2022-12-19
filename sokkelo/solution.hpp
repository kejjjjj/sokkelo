#pragma once

#ifndef __sol
#define __sol

#include "sokkelo.h"

inline bool SOLUTION_DRAW_DEADENDS = true;

//even though the solution can be found when building the maze,
//the point of this project is to test AI, so I will not give the AI any solutions
//and instead I will let it learn by itself :)
class Solution
{
public:

	void SolveTheMaze();

	void Initialize();
	void Restart();
	void OnExit();

	static void DoStuffWrapper();
	void DoStuff();

	bool Initialized() { return bInitialized; }
	bool Finished() { return bFinished; }
	Maze::sCell* sCurrentTile;
	Maze::sCell* sPreviousTile;
	std::list<Maze::sCell*> vCurrentCorridor; //if the scan hits a deadend, mark every cell here as a deadend
	std::list<Maze::sCell*> vFinishPath;
	uint64_t iGenerations = 0; //how many times has the AI tried to solve this maze

private:

	bool EvaluateNextMove();
	bool GetPossiblePaths(); //returns false if at a deadend
	Maze::sCell* GuessBestPath();

	std::list<Maze::eDir> vPossiblePaths; //directions, where the AI could advance from the current tile
	//std::vector<Maze::sCell*> vPath; //what blocks I've already visited during this generation, stops the ai from moving backwards
	ImVec2 vEndTile;
	bool bFinished = false;
	bool bInitialized = false;
	Maze::eDir eLastMove = Maze::eDir::INVALID;

};
inline Solution solution;
#endif