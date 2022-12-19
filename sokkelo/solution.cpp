#include "sokkelo.h"


void Solution::SolveTheMaze()
{
	
	Initialize();


}
void Solution::Initialize()
{
	iGenerations = NULL;
	vEndTile = ui.vCells.back().vPos;
	sCurrentTile = &ui.vCells.front();
	vFinishPath.clear();
	vCurrentCorridor.clear();
	vPossiblePaths.clear();
	
	bFinished = false;
	bInitialized = true;
}
void Solution::DoStuffWrapper()
{
	
	return solution.DoStuff();
}
void Solution::DoStuff()
{
	if (!Initialized() || bFinished)
		return;

	//std::chrono::time_point<std::chrono::system_clock> old = std::chrono::system_clock::now();

	while (!bFinished && bInitialized) {

		if (EvaluateNextMove()) {
			vCurrentCorridor.push_back(sCurrentTile);
			vFinishPath.push_back(sCurrentTile);
			//std::this_thread::sleep_for(1ns);
		}
		
	}
	//std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	//std::chrono::duration<double> difference = now - old;


	//printf("time taken: %.12f\n", difference.count());

	//Sleep(1);
}
//restarts the search, but everytime this is called, the AI is more knowledgeable of the route
void Solution::Restart()
{
	vEndTile = ui.vCells.back().vPos;
	sCurrentTile = &ui.vCells.front();
	iGenerations++;
	vCurrentCorridor.clear();
	vCurrentCorridor.resize(0);
	vFinishPath.clear();
	vFinishPath.resize(0);
	
}
void Solution::OnExit()
{
	vEndTile = ImVec2();
	sCurrentTile = 0;
	sPreviousTile = 0;
	iGenerations = 0;
	vCurrentCorridor.clear();
	vCurrentCorridor.resize(0);
	vFinishPath.clear();
	vFinishPath.resize(0);
	vPossiblePaths.clear();
	vPossiblePaths.resize(0);
	bFinished = false;
	bInitialized = false;
}
bool Solution::EvaluateNextMove()
{

	if (!GetPossiblePaths()) {
		//currently at a deadend
		//mark all cells in this corridor as deadends
		std::for_each(vCurrentCorridor.begin(), vCurrentCorridor.end(), [](Maze::sCell* cell) {cell->bPathLeadsToDeadend = true; });

		Restart();
		return false;
	}

	//evaluate which path should be taken

	sCurrentTile = GuessBestPath();

	if (sCurrentTile == &ui.vCells.back()) {
		std::cout << "YEP!\n";
		bFinished = true;
	}

	return true;
}
bool Solution::GetPossiblePaths()
{
	Maze::sCell* sc;

	vPossiblePaths.clear();
	vPossiblePaths.resize(0);

	//find a neighbor
	//if it is not a wall: store it
	sc = sCurrentTile->vAllNeighbors[(int)Maze::eDir::N]; if (sc) if (!sc->bWall && !sc->bPathLeadsToDeadend && sc != sPreviousTile) { vPossiblePaths.push_back(Maze::eDir::N); }
	sc = sCurrentTile->vAllNeighbors[(int)Maze::eDir::E]; if (sc) if (!sc->bWall && !sc->bPathLeadsToDeadend && sc != sPreviousTile) { vPossiblePaths.push_back(Maze::eDir::E); }
	sc = sCurrentTile->vAllNeighbors[(int)Maze::eDir::S]; if (sc) if (!sc->bWall && !sc->bPathLeadsToDeadend && sc != sPreviousTile) { vPossiblePaths.push_back(Maze::eDir::S); }
	sc = sCurrentTile->vAllNeighbors[(int)Maze::eDir::W]; if (sc) if (!sc->bWall && !sc->bPathLeadsToDeadend && sc != sPreviousTile) { vPossiblePaths.push_back(Maze::eDir::W); }

	return vPossiblePaths.size() > 0 || vPossiblePaths.size() == 1 && sCurrentTile->iIndex == 0;

}
Maze::sCell* Solution::GuessBestPath()
{
	if (vPossiblePaths.size() == 1) {
		if (static_cast<Maze::eDir>(vPossiblePaths.front()) != eLastMove) {

			vCurrentCorridor.clear();
			vCurrentCorridor.resize(0);
		}
		eLastMove = static_cast<Maze::eDir>(vPossiblePaths.front());

		sPreviousTile = sCurrentTile;
		return ui.GetCellNeigbor(*sCurrentTile, static_cast<Maze::eDir>(vPossiblePaths.front()), false);
	}

	std::list<std::pair<float, Maze::eDir>> distances;

	int closest_index = 0;

	//get the distance to each direction
	for (const auto& i : vPossiblePaths) {
		const Maze::sCell* cell = sCurrentTile->vAllNeighbors[(int)i];
		const ImVec2 subtracted = ImVec2(vEndTile.x - cell->vPos.x, vEndTile.y - cell->vPos.y);
		distances.push_back(std::make_pair((subtracted.x * subtracted.x + subtracted.y * subtracted.y), i));
	}

	//prefer the route that takes you closer to the end tile
	//get the index closest to the end
	float lowest = distances.front().first;
	const size_t size = distances.size();
	for (const auto& i : distances) {

		if (i.first <= lowest) {
			lowest = i.first;
			closest_index = (int)i.second;
			//closest_index.insert(closest_index.begin(), distances[i].second);
		}

	}

	//an intersection, so clear the whole thing
	vCurrentCorridor.clear();
	
	eLastMove = static_cast<Maze::eDir>(closest_index);
	Maze::sCell* BestNeigbor = sCurrentTile->vAllNeighbors[closest_index];

	//skip routes that are known to be deadends
	//this should always be false
	//if (BestNeigbor->bPathLeadsToDeadend) {
	//	vPossiblePaths.erase(vPossiblePaths.begin() + closest_index, vPossiblePaths.begin() + closest_index + 1);
	//	return GuessBestPath(); //recursively find the next best path
	//}

	sPreviousTile = sCurrentTile;

	return BestNeigbor;
}