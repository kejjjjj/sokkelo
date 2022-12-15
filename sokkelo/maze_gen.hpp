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
		ImVec2 vPos; //position in the grid
	};
	static void IterativeGenerationWrapper(const int& index);

	void IterativeGeneration(const int& index);

	std::vector<sCell> vCells;
private:
	void PopulateCells();
	void PopulateCellNeighbors();
	sCell* GetCellNeigbor(const sCell& cell, const eDir& dir);


	

};

inline Maze ui;

#endif