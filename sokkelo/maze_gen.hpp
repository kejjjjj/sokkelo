#pragma once

#ifndef _maze
#define _maze

#include "sokkelo.h"


class Maze : public AppUI
{
public:

	enum class eDir
	{
		N, E, S, W
	};

	struct sCell
	{

		int iIndex; //index on the grid, left to right, top to bottom order
		std::array<sCell*, 4> vNeighbors;
		bool bVisited; //the cell has already been visited
		bool bWall; //current cell is a wall
		bool bBacktraced; //deadend
		ImVec2 vPos; //position in the grid
	};
	std::thread generation_thread;
	static void IterativeGenerationWrapper(const int& index);

	void IterativeGeneration(const int& index);
	void KillGeneration();

	std::vector<sCell> vCells;
	bool bAbleToRender = false;
	bool bThreadActive = false;
private:
	void PopulateCells();
	void PopulateCellNeighbors();
	sCell* GetCellNeigbor(const sCell& cell, const eDir& dir, const bool bSkipWalls = true);
	sCell* GetCellInBetween(const sCell& a, const sCell& b);

	

};

inline Maze ui;

#endif