#include "sokkelo.h"

void Maze::PopulateCells()
{
	int cellIdx = 0;

	sCell cell;

	for (int y = 0; y < iPixelsPerAxis.y; y++) {

		for (int x = 0; x < iPixelsPerAxis.x; x++) {

			cell.bWall = true;
			cell.bVisited = false;
			cell.iIndex = cellIdx;
			cell.vPos = ImVec2(x, y);
			cell.bBacktraced = false;
			//cell.vNeighbors.fill(nullptr);

			vCells.push_back(cell);

			cellIdx++;
		}

	}
	

}
void Maze::PopulateCellNeighbors()
{
	for (sCell& cell : vCells) {

		cell.vNeighbors[(int)eDir::N] = GetCellNeigbor(cell, eDir::N);
		cell.vNeighbors[(int)eDir::E] = GetCellNeigbor(cell, eDir::E);
		cell.vNeighbors[(int)eDir::S] = GetCellNeigbor(cell, eDir::S);
		cell.vNeighbors[(int)eDir::W] = GetCellNeigbor(cell, eDir::W);

	}
}
Maze::sCell* Maze::GetCellNeigbor(const sCell& cell, const eDir& dir, const bool bSkipWalls)
{
	int cellIndex = 0;
	const size_t size = vCells.size();
	const int32_t multiplier = bSkipWalls == true ? 2 : 1;
	switch (dir) {

	case eDir::N: //above

		//subtract iPixelsPerAxis*2 from the axis 

		cellIndex = cell.iIndex - iPixelsPerAxis.x * multiplier;

		if (cellIndex < 0) //current cell is at the top level so it does not have a top neighbor
			return nullptr;
	

		return &vCells[cellIndex];

	case eDir::E: //right

		
		//0 HAS to have a right neighbor unless it's a ?x2 grid
		if (cell.iIndex != 0) {

			int remainder = cell.iIndex % (iPixelsPerAxis.x);

			//if the remainder is 0, then this cell is at the right edge
			if (remainder == iPixelsPerAxis.x - 1) {
				return 0;
			}
			if (cell.iIndex % 2 != 0 && bSkipWalls) {
				return 0;
			}

		}

		cellIndex = cell.iIndex + multiplier;

		if (cellIndex >= size)
			return nullptr;
		
		return &vCells[cellIndex];

	case eDir::S: //below

		cellIndex = cell.iIndex + iPixelsPerAxis.x * multiplier;
		
		if (cellIndex >= size) //current cell doesn't have a below neighbor if it exceeds the vCells size
			return nullptr;

		return &vCells[cellIndex];

	case eDir::W: //left

		//if the remainder is 0, then this cell is at the left edge
		if (cell.iIndex % iPixelsPerAxis.x == 0) {
			return 0;
		}

		if (cell.iIndex % 2 != 0 && bSkipWalls) {
			return 0;
		}

		cellIndex = cell.iIndex - multiplier;

		if (cellIndex < 0)
			return nullptr;

		return &vCells[cellIndex];
	}


	return nullptr;
}

Maze::sCell* Maze::GetCellInBetween(const sCell& a, const sCell& b)
{
	const int firstIndex = a.iIndex;
	const int secondIndex = b.iIndex;



	//on the right
	if (secondIndex - firstIndex == 2) {
		return &vCells[firstIndex + 1];
	}

	//on the left
	else if (secondIndex - firstIndex == -2) {
		return &vCells[firstIndex - 1];
	}
	//below
	else if (secondIndex - firstIndex == iPixelsPerAxis.x * 2) {

		

		return &vCells[firstIndex + iPixelsPerAxis.x];

		

	}
	//above
	else if (secondIndex - firstIndex == -(iPixelsPerAxis.x * 2)) {
		return &vCells[firstIndex - iPixelsPerAxis.x];

	}
	FatalError("Can't find a cell between ", firstIndex, " and ", secondIndex);
	return 0;
}
void Maze::SetAlgorithm(const eMazeAlgorithm& alg)
{
	algorithm = alg;
}
Maze::eMazeAlgorithm Maze::GetAlgorithm()
{
	return algorithm;
}
void Maze::StartGeneration()
{
	switch (algorithm) {
	case eMazeAlgorithm::Depth_First:
		ui.generation_thread = std::thread(ui.IterativeGenerationWrapper, 0);
		break;

	case eMazeAlgorithm::Aldous_Broder:
		ui.generation_thread = std::thread(ui.AldousBroderAlgorithmWrapper, 0);
		break;

	default:
		break;
	}


}
void Maze::IterativeGenerationWrapper(const int& index)
{
	return ui.IterativeGeneration(index);
}
void Maze::AldousBroderAlgorithmWrapper(const int& index)
{
	return ui.AldousBroderAlgorithm(index);
}
void Maze::IterativeGeneration(const int& index) //index = starting position
{
	bThreadActive = true;
	bAbleToRender = false;
	vCells.clear();

	PopulateCells();
	PopulateCellNeighbors();

	if (index >= vCells.size() || vCells.size() >= vCells.max_size()) {
		FatalError("Maze::IterativeGeneration(): passed index: ", index, " > ", vCells.size());
		return;
	}
	srand(time(NULL));
	ui.generation_thread.detach();
	bAbleToRender = true;
 
	std::vector<sCell*> stack;
	std::vector<size_t> valid_neighbors;

	stack.push_back(&vCells[index]);
	vCells[index].bWall = false;

	sCell* sCurrentCell;
	while (!stack.empty()) {
		bool backtraced = true;
		valid_neighbors.clear();

		if (stack.empty() || !bThreadActive)
			break;

		sCurrentCell = stack.back();
		sCurrentCell->bVisited = true;
		//sCurrentCell->bBacktraced = true;
		stack.pop_back();

		//get available neighbors
		for (size_t i = 0; i < 4; i++) {
			if (sCurrentCell->vNeighbors[i]) {
				if (!sCurrentCell->vNeighbors[i]->bVisited)
					valid_neighbors.push_back(i); //has not been visited so push it to the stack
			}
		}

		if (valid_neighbors.size() > 0) {
			backtraced = false;
			sCurrentCell->bBacktraced = false;
			//get a random neighbor
			int rand_idx = valid_neighbors[rand() % (valid_neighbors.size())];

			//Push the current cell to the stack
			stack.push_back(sCurrentCell);

			//Choose one of the unvisited neighbours
			sCell* sChosenCell = sCurrentCell->vNeighbors[rand_idx];

			//Remove the wall between the current cell and the chosen cell
			sCell* inBetween = GetCellInBetween(*sCurrentCell, *sChosenCell);
			if (inBetween) {
				inBetween->bVisited = true;
				inBetween->bWall = false;

			}

			//Mark the chosen cell as visited and push it to the stack
			sChosenCell->bVisited = true;
			stack.push_back(sChosenCell);


		}
		//if (!backtraced)
		//	std::this_thread::sleep_for(500us);
		

	}
	bThreadActive = false;

	return;

}
void Maze::AldousBroderAlgorithm(const int& index)
{
	bThreadActive = true;
	bAbleToRender = false;
	vCells.clear();

	PopulateCells();
	PopulateCellNeighbors();

	if (index >= vCells.size() || vCells.size() >= vCells.max_size()) {
		FatalError("Maze::AldousBroderAlgorithm(): passed index: ", index, " > ", vCells.size());
		return;
	}
	srand(time(NULL));
	ui.generation_thread.detach();
	bAbleToRender = true;

	std::vector<size_t> valid_neighbors;

	//Pick a random cell as the current cell and mark it as visited.
	vCells[index].bWall = false;
	vCells[index].bVisited = true;

	sCell* sCurrentCell = &vCells[index];

	int neighbors_left_to_visit = vCells.size();
	size_t visited_cells = 0;
	DWORD looped = 0;
	//While there are unvisited cells:
	while (neighbors_left_to_visit) {

		bool backtraced = true;
		valid_neighbors.clear();

		if (!bThreadActive)
			break;

		//get available neighbors
		for (size_t i = 0; i < 4; i++) {
			if (sCurrentCell->vNeighbors[i]) {
					valid_neighbors.push_back(i); 
			}
		}


		//Pick a random neighbour.
		const int rand_idx = valid_neighbors[rand() % (valid_neighbors.size())];
		sCell* sChosenCell = sCurrentCell->vNeighbors[rand_idx];

		if (!bThreadActive)
			break;

		//If the chosen neighbour has not been visited:
		if (!sChosenCell->bVisited) {
			backtraced = false;
			//Remove the wall between the current cell and the chosen neighbour.
			sCell* inBetween = GetCellInBetween(*sCurrentCell, *sChosenCell);
			if (inBetween) {
				inBetween->bVisited = true;
				inBetween->bWall = false;
				--neighbors_left_to_visit;
				++visited_cells;
			}

			//Mark the chosen neighbour as visited.
			sChosenCell->bVisited = true;
			--neighbors_left_to_visit;
			++visited_cells;
			looped = 0;
		}
		else
			looped++;
		

		if (looped > 10000000)
			break;

		//Make the chosen neighbour the current cell.
		sCurrentCell = sChosenCell;

		//if (!backtraced)
		//	std::this_thread::sleep_for(50us);
	}

	bThreadActive = false;

}
void Maze::KillGeneration()
{
	ui.bThreadActive = false;
	//ui.generation_thread.~thread();
	Sleep(1);
	vCells.clear();


}