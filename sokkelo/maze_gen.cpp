#include "sokkelo.h"

void Maze::PopulateCells()
{
	int cellIdx = 0;

	sCell cell;

	for (int y = 0; y < iPixelsPerAxis; y++) {

		for (int x = 0; x < iPixelsPerAxis; x++) {

			cell.bWall = true;
			cell.bVisited = false;
			cell.iIndex = cellIdx;
			cell.vPos = ImVec2(x, y);
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
Maze::sCell* Maze::GetCellNeigbor(const sCell& cell, const eDir& dir)
{
	int cellIndex = 0;
	const size_t size = vCells.size();
	switch (dir) {

	case eDir::N: //above

		//subtract iPixelsPerAxis*2 from the axis 

		cellIndex = cell.iIndex - iPixelsPerAxis * 2;

		if (cellIndex < 0) //current cell is at the top level so it does not have a top neighbor
			return nullptr;
	

		return &vCells[cellIndex];

	case eDir::E: //right

		
		//0 HAS to have a right neighbor unless it's a ?x2 grid
		if (cell.iIndex != 0) {

			//if the remainder is 0, then this cell is at the right edge
			if (cell.iIndex % (iPixelsPerAxis - 1) == 0) {
				return 0;
			}
			if ((cell.iIndex % (iPixelsPerAxis - 1)) % 2 != 0) {
				return 0;
			}

		}

		cellIndex = cell.iIndex + 2;

		if (cellIndex >= size)
			return nullptr;
		
		return &vCells[cellIndex];

	case eDir::S: //below

		cellIndex = cell.iIndex + iPixelsPerAxis * 2;
		
		if (cellIndex >= size) //current cell doesn't have a below neighbor if it exceeds the vCells size
			return nullptr;

		return &vCells[cellIndex];

	case eDir::W: //left


		//if the remainder is 0, then this cell is at the left edge
		if (cell.iIndex % iPixelsPerAxis == 0) {
			return 0;
		}

		if ((cell.iIndex % (iPixelsPerAxis - 1)) % 2 != 0) {
			return 0;
		}

		cellIndex = cell.iIndex - 2;

		if (cellIndex < 0)
			return nullptr;

		return &vCells[cellIndex];
	}


	return nullptr;
}
void Maze::IterativeGenerationWrapper(const int& index)
{
	return ui.IterativeGeneration(index);
}
void Maze::IterativeGeneration(const int& index) //index = starting position
{
	vCells.clear();

	PopulateCells();
	PopulateCellNeighbors();

	if (index >= vCells.size()) {
		FatalError("Maze::IterativeGeneration(): passed index: ", index, " > ", vCells.size());
		return;
	}

	std::vector<sCell*> stack;
	std::vector<size_t> valid_neighbors;

	stack.push_back(&vCells[index]);


	sCell* sCurrentCell;
	int aaa = 0;
	while (!stack.empty() && aaa < (vCells.size() / 50)) {
		aaa++;
		valid_neighbors.clear();

		if (stack.empty())
			break;

		sCurrentCell = stack.back();
		sCurrentCell->bVisited = true;

		stack.pop_back();

		//get available neighbors
		for (size_t i = 0; i < 4; i++) {
			if (sCurrentCell->vNeighbors[i]) {
				if (!sCurrentCell->vNeighbors[i]->bVisited)
					valid_neighbors.push_back(i); //has not been visited so push it to the stack
			}
		}

		if (valid_neighbors.size() > 0) {

			//get a random neighbor
			int rand_idx = valid_neighbors[rand() % (valid_neighbors.size())];

			//Push the current cell to the stack
			stack.push_back(sCurrentCell);

			//Choose one of the unvisited neighbours
			sCell* sChosenCell = sCurrentCell->vNeighbors[rand_idx];

			//Remove the wall between the current cell and the chosen cell
			ImVec2 angles = VectorsToAngles(sCurrentCell->vPos, sChosenCell->vPos);
			float angle = atan2(angles.y, angles.x) * 180.f / M_PI;



			//Mark the chosen cell as visited and push it to the stack
			sChosenCell->bVisited = true;
			stack.push_back(sChosenCell);


		}
		Sleep(250);
		

	}

	return;

}
