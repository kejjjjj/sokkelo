#pragma once

#ifndef _maze
#define _maze

#include "sokkelo.h"


class Maze : public AppUI
{
public:

	enum class eMazeAlgorithm {
		Depth_First,
		Aldous_Broder
	};

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
		bool bAlreadyMerged; //
	};
	std::thread generation_thread;
	void StartGeneration();
	static void IterativeGenerationWrapper(const int& index);
	static void AldousBroderAlgorithmWrapper(const int& index);

	void IterativeGeneration(const int& index);
	void AldousBroderAlgorithm(const int& index);
	void SetAlgorithm(const eMazeAlgorithm& alg);
	eMazeAlgorithm GetAlgorithm();

	void KillGeneration();

	std::vector<sCell> vCells;
	bool bAbleToRender = false;
	bool bThreadActive = false;
	sCell* GetCellNeigbor(const sCell& cell, const eDir& dir, const bool bSkipWalls = true);

private:
	void PopulateCells();
	void PopulateCellNeighbors();
	sCell* GetCellInBetween(const sCell& a, const sCell& b);
	eMazeAlgorithm algorithm = eMazeAlgorithm::Depth_First;
	

};

inline Maze ui;

#endif