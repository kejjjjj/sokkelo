#include "sokkelo.h"


void Solution::SolveTheMaze()
{
	
	Initialize();


}
void Solution::Initialize()
{
	iGenerations = NULL;
	vEndTile = vCells.back().vPos;
	sCurrentTile = &vCells.front();
	bFinished = false;
}

//restarts the search, but everytime this is called, the AI is more knowledgeable of the route
void Solution::Restart()
{

}

const Maze::sCell* Solution::EvaluateNextMove()
{

	if (!GetPossiblePaths()) {
		//currently at a deadend
		//mark all cells in this corridor as deadends
		std::for_each(vCurrentCorridor.begin(), vCurrentCorridor.end(), [](sCell* cell) {cell->bPathLeadsToDeadend = true; });

		Restart();
	}

	//evaluate which path should be taken

	return GuessBestPath();


}
bool Solution::GetPossiblePaths()
{
	sCell* sc;

	vPossiblePaths.clear();
	vPossiblePaths.resize(1);

	//find a neighbor
	//if it is not a wall: store it
	sc = GetCellNeigbor(*sCurrentTile, eDir::N, false); if (sc) if (!sc->bWall) { vPossiblePaths.push_back(eDir::N); }
	sc = GetCellNeigbor(*sCurrentTile, eDir::E, false); if (sc) if (!sc->bWall) { vPossiblePaths.push_back(eDir::E); }
	sc = GetCellNeigbor(*sCurrentTile, eDir::S, false); if (sc) if (!sc->bWall) { vPossiblePaths.push_back(eDir::S); }
	sc = GetCellNeigbor(*sCurrentTile, eDir::W, false); if (sc) if (!sc->bWall) { vPossiblePaths.push_back(eDir::W); }

	return vPossiblePaths.size() > 1;

}
const Maze::sCell* Solution::GuessBestPath()
{
	

	std::vector<float> distances;
	std::vector<int> closest_index;

	//get the distance to each direction
	for (const auto& i : vPossiblePaths) {
		const sCell* cell = GetCellNeigbor(*sCurrentTile, i, false);
		const ImVec2 subtracted = ImVec2(vEndTile.x - cell->vPos.x, vEndTile.y - cell->vPos.y);
		distances.push_back(sqrtf(subtracted.x * subtracted.x + subtracted.y * subtracted.y));

	}

	//prefer the route that takes you closer to the end tile
	//get the index closest to the end
	float lowest = distances.front();

	for (int i = 1; i < distances.size(); i++) {
		if (distances[i] < lowest) {
			lowest = distances[i];
			closest_index.insert(closest_index.begin(), i);
		}

	}

	const sCell* BestNeigbor = GetCellNeigbor(*sCurrentTile, static_cast<eDir>(closest_index[0]), false);

	//skip routes that are known to be deadends
	if (BestNeigbor->bPathLeadsToDeadend) {
		vPossiblePaths.erase(vPossiblePaths.begin() + closest_index[0], vPossiblePaths.begin() + closest_index[0] + 1);
		return GuessBestPath(); //recursively find the next best path
	}

	return BestNeigbor;
}