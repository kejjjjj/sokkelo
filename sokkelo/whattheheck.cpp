
#include "sokkelo.h"

void CoD4::WriteMapRelatedUnnecessaryStuff()
{

	f << "iwmap 4\n";
	f << '"' << "000_Global" << '"' << " flags  active\n";
	f << '"' << "The Map" << '"' << " flags\n";
	f << "// entity 0\n{\n";
	f << '"' << "classname" << '"' << ' ' << '"' << "worldspawn" << '"' << '\n';

}
void CoD4::CellToBounds(const Maze::sCell* cell, brush_t& brush)
{
	const ImVec2 pos = cell->vPos;


	if (cell->bVisited)
		brush.bWalkable = true;
	else
		brush.bWalkable = false;

	const float mins = cell->bVisited == false ? 0 : -brushsize / 2;
	const float maxs = cell->bVisited == false ? brushsize : 0;
	const float length = brushsize / 2;
	
	brush.origin[0] = (pos.x * brushsize);
	brush.origin[1] = (pos.y * brushsize);
	brush.origin[2] = brushsize / 2;


	brush.mins[0] = brush.origin[0] - length;
	brush.mins[1] = brush.origin[1] - length;
	brush.mins[2] = mins;


	brush.maxs[0] = brush.origin[0] + length;
	brush.maxs[1] = brush.origin[1] + length;
	brush.maxs[2] = maxs;



}
void CoD4::WriteBrush(const brush_t& brush, const int brushIndex)
{
	float brush_1[3][3];
	float brush_2[3][3];
	float brush_3[3][3];
	float brush_4[3][3];
	float brush_5[3][3];
	float brush_6[3][3];

	vec3_t mins, maxs;

	VectorCopy(brush.mins, mins);
	VectorCopy(brush.maxs, maxs);

	//copy pasted from an old project :x
	{
		brush_1[0][0] = maxs[0];
		brush_1[0][1] = maxs[1];
		brush_1[0][2] = mins[2];

		brush_1[1][0] = mins[0];
		brush_1[1][1] = maxs[1];
		brush_1[1][2] = mins[2];

		brush_1[2][0] = mins[0];
		brush_1[2][1] = mins[1];
		brush_1[2][2] = mins[2];
	}
	{
		brush_2[0][0] = mins[0];
		brush_2[0][1] = mins[1];
		brush_2[0][2] = maxs[2];

		brush_2[1][0] = mins[0];
		brush_2[1][1] = maxs[1];
		brush_2[1][2] = maxs[2];

		brush_2[2][0] = maxs[0];
		brush_2[2][1] = maxs[1];
		brush_2[2][2] = maxs[2];
	} //
	{
		brush_3[0][0] = mins[0];
		brush_3[0][1] = mins[1];
		brush_3[0][2] = maxs[2];

		brush_3[1][0] = maxs[0];
		brush_3[1][1] = mins[1];
		brush_3[1][2] = maxs[2];

		brush_3[2][0] = maxs[0];
		brush_3[2][1] = mins[1];
		brush_3[2][2] = mins[2];
	}
	{
		brush_4[0][0] = maxs[0];
		brush_4[0][1] = mins[1];
		brush_4[0][2] = maxs[2];

		brush_4[1][0] = maxs[0];
		brush_4[1][1] = maxs[1];
		brush_4[1][2] = maxs[2];

		brush_4[2][0] = maxs[0];
		brush_4[2][1] = maxs[1];
		brush_4[2][2] = mins[2];
	}
	{
		brush_5[0][0] = maxs[0];
		brush_5[0][1] = maxs[1];
		brush_5[0][2] = maxs[2];

		brush_5[1][0] = mins[0];
		brush_5[1][1] = maxs[1];
		brush_5[1][2] = maxs[2];

		brush_5[2][0] = mins[0];
		brush_5[2][1] = maxs[1];
		brush_5[2][2] = mins[2];
	}
	{
		brush_6[0][0] = mins[0];
		brush_6[0][1] = maxs[1];
		brush_6[0][2] = maxs[2];

		brush_6[1][0] = mins[0];
		brush_6[1][1] = mins[1];
		brush_6[1][2] = maxs[2];

		brush_6[2][0] = mins[0];
		brush_6[2][1] = mins[1];
		brush_6[2][2] = mins[2];
	}

	std::string texture = brush.bWalkable == true ? "ch_brick_wall_08" : "ch_brick_wall_07";

	if (brushIndex == 0)
		texture = "icbm_hazardstripe2";
	else if (brushIndex == ui.vCells.size() - 1)
		texture = "icbm_freightelevator";

	f << "// brush " << brushIndex << std::endl;
	f << "{\n";

	//THIS IS BOTTOM [1][2]
	f << std::fixed << " ( " << brush_1[0][0] << " " << brush_1[0][1] << " " << brush_1[0][2] << " )";
	f << std::fixed << " ( " << brush_1[1][0] << " " << brush_1[1][1] << " " << brush_1[1][2] << " )";
	f << std::fixed << " ( " << brush_1[2][0] << " " << brush_1[2][1] << " " << brush_1[2][2] << " ) " << texture << " 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0\n";

	f << std::fixed << " ( " << brush_2[0][0] << " " << brush_2[0][1] << " " << brush_2[0][2] << " )";
	f << std::fixed << " ( " << brush_2[1][0] << " " << brush_2[1][1] << " " << brush_2[1][2] << " )";
	f << std::fixed << " ( " << brush_2[2][0] << " " << brush_2[2][1] << " " << brush_2[2][2] << " ) " << texture << " 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0\n";

	f << std::fixed << " ( " << brush_3[0][0] << " " << brush_3[0][1] << " " << brush_3[0][2] << " )";
	f << std::fixed << " ( " << brush_3[1][0] << " " << brush_3[1][1] << " " << brush_3[1][2] << " )";
	f << std::fixed << " ( " << brush_3[2][0] << " " << brush_3[2][1] << " " << brush_3[2][2] << " ) " << texture << " 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0\n";

	f << std::fixed << " ( " << brush_4[0][0] << " " << brush_4[0][1] << " " << brush_4[0][2] << " )";
	f << std::fixed << " ( " << brush_4[1][0] << " " << brush_4[1][1] << " " << brush_4[1][2] << " )";
	f << std::fixed << " ( " << brush_4[2][0] << " " << brush_4[2][1] << " " << brush_4[2][2] << " ) " << texture << " 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0\n";

	f << std::fixed << " ( " << brush_5[0][0] << " " << brush_5[0][1] << " " << brush_5[0][2] << " )";
	f << std::fixed << " ( " << brush_5[1][0] << " " << brush_5[1][1] << " " << brush_5[1][2] << " )";
	f << std::fixed << " ( " << brush_5[2][0] << " " << brush_5[2][1] << " " << brush_5[2][2] << " ) " << texture << " 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0\n";


	f << std::fixed << " ( " << brush_6[0][0] << " " << brush_6[0][1] << " " << brush_6[0][2] << " )";
	f << std::fixed << " ( " << brush_6[1][0] << " " << brush_6[1][1] << " " << brush_6[1][2] << " )";
	f << std::fixed << " ( " << brush_6[2][0] << " " << brush_6[2][1] << " " << brush_6[2][2] << " ) " << texture << " 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0\n";


	f << "}\n";


}
void CoD4::BeginConversion()
{
	if (!mazedata) {
		FatalError("BeginConversion(): called with invalid maze data");
		return;
	}

	if (!fs::F_OpenFile(f, path, fs::fileopen::FILE_OUT)) {
		FatalError("CoD4::BeginConversion(): can't open file for OUT operation! Error: ", fs::_GetLastError());
		return;
	}

	WriteMapRelatedUnnecessaryStuff();

	brush_t brush;

	size_t i = 0;
	for (const auto& cell : ui.vCells) {
		CellToBounds(&cell, brush);
		WriteBrush(brush, i);
		i++;
	}


	f << "}\n";

	fs::F_CloseFile(f);
}