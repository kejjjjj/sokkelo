
#include "sokkelo.h"

void CoD4::WriteMapRelatedUnnecessaryStuff()
{

	f << "iwmap 4\n";
	f << "\"000_Global\" flags  active\n";
	f << "\"The Map\" flags\n";
	f << "// entity 0\n{\n";
	f << "\"contrastGain\" " "\"0.125\"" << '\n';
	f << "\"diffusefraction\" " "\"0.5\"" << '\n';
	f << "\"_color\" " "\"0.2 0.27 0.3 1\"" << '\n';
	f << "\"sunlight\" " "\"1\"" << '\n';
	f << "\"sundirection\" " "\"-30 -95 0\"" << '\n';
	f << "\"sundiffusecolor\" " "\"0.2 0.27 0.3 1\"" << '\n';
	f << "\"suncolor\" " "\"0.2 0.27 0.3 1\"" << '\n';
	f << "\"ambient\" " "\".1\"" << '\n';
	f << "\"bouncefraction\" " "\".7\"" << '\n';
	f << "\"classname\" \"worldspawn\"\n";

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
	brush.origin[2] = 0;


	brush.mins[0] = brush.origin[0] - length;
	brush.mins[1] = brush.origin[1] - length;
	brush.mins[2] = mins;


	brush.maxs[0] = brush.origin[0] + length;
	brush.maxs[1] = brush.origin[1] + length;
	brush.maxs[2] = maxs;

	if (EXPORT_RISING_WALLS) {
		if (!brush.bWalkable) {
			brush.maxs[2] = sqrt(brush.origin[0] * brush.origin[0] + brush.origin[1] * brush.origin[1]) / 5;
			if (brush.maxs[2] < brushsize)
				brush.maxs[2] = brushsize;
		}
	}

}
void CoD4::CellsToBounds(Maze::sCell* cell, brush_t& brush, std::vector<Maze::sCell>::iterator& it)
{
	if (it >= ui.vCells.end() - 1)
		return;

	eTextures direction = eTextures::INVALID;
	brush_t b2, backup;
	
	CellToBounds(cell, brush);

	++it;

	//if (it->bAlreadyMerged)
	//	return;


	if (it >= ui.vCells.end()-1)
		return;

	CellToBounds(&*it, b2);

	if (brush.maxs[1] == b2.maxs[1])
		direction = eTextures::posX;


	if (direction == eTextures::INVALID || brush.maxs[2] != b2.maxs[2]) {
		--it;
		return;
	}
	--it;
	const int index = direction == eTextures::posX ? 1 : 0; 

	backup = brush;
	int times_looped = 0;
	float ADD = 0;
	while(true){
		++it;

		if (it >= ui.vCells.end() - 1)
			break;

		CellToBounds(&*it, b2);

		if (brush.maxs[index] != b2.maxs[index] || brush.maxs[2] != b2.maxs[2]) {
			--it;
			break;
		}

		backup = b2;
		times_looped++;
		ADD += brushsize;
		//it->bAlreadyMerged = true;
	}

	brush.maxs[!index] += ADD;
	//cell->bAlreadyMerged = true;
	
}
void CoD4::WriteBrush(const brush_t& brush, const int brushIndex, std::array<const char*, 6>& textures)
{


	f << "// brush " << brushIndex << std::endl;
	f << "{\n";

	WriteBrushContents(brush, textures);

	f << "}\n";

	ibrushIndex++;

}
void CoD4::WriteBrushContents(const brush_t& brush, std::array<const char*, 6>& textures)
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

	//THIS IS BOTTOM [1][2]
	f << std::fixed << " ( " << brush_1[0][0] << " " << brush_1[0][1] << " " << brush_1[0][2] << " )";
	f << std::fixed << " ( " << brush_1[1][0] << " " << brush_1[1][1] << " " << brush_1[1][2] << " )";
	f << std::fixed << " ( " << brush_1[2][0] << " " << brush_1[2][1] << " " << brush_1[2][2] << " ) " << textures[(int)eTextures::negZ] << " 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0\n";

	f << std::fixed << " ( " << brush_2[0][0] << " " << brush_2[0][1] << " " << brush_2[0][2] << " )";
	f << std::fixed << " ( " << brush_2[1][0] << " " << brush_2[1][1] << " " << brush_2[1][2] << " )";
	f << std::fixed << " ( " << brush_2[2][0] << " " << brush_2[2][1] << " " << brush_2[2][2] << " ) " << textures[(int)eTextures::posZ] << " 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0\n";

	f << std::fixed << " ( " << brush_3[0][0] << " " << brush_3[0][1] << " " << brush_3[0][2] << " )";
	f << std::fixed << " ( " << brush_3[1][0] << " " << brush_3[1][1] << " " << brush_3[1][2] << " )";
	f << std::fixed << " ( " << brush_3[2][0] << " " << brush_3[2][1] << " " << brush_3[2][2] << " ) " << textures[(int)eTextures::posY] << " 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0\n";

	f << std::fixed << " ( " << brush_4[0][0] << " " << brush_4[0][1] << " " << brush_4[0][2] << " )";
	f << std::fixed << " ( " << brush_4[1][0] << " " << brush_4[1][1] << " " << brush_4[1][2] << " )";
	f << std::fixed << " ( " << brush_4[2][0] << " " << brush_4[2][1] << " " << brush_4[2][2] << " ) " << textures[(int)eTextures::negX] << " 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0\n";

	f << std::fixed << " ( " << brush_5[0][0] << " " << brush_5[0][1] << " " << brush_5[0][2] << " )";
	f << std::fixed << " ( " << brush_5[1][0] << " " << brush_5[1][1] << " " << brush_5[1][2] << " )";
	f << std::fixed << " ( " << brush_5[2][0] << " " << brush_5[2][1] << " " << brush_5[2][2] << " ) " << textures[(int)eTextures::negY] << " 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0\n";


	f << std::fixed << " ( " << brush_6[0][0] << " " << brush_6[0][1] << " " << brush_6[0][2] << " )";
	f << std::fixed << " ( " << brush_6[1][0] << " " << brush_6[1][1] << " " << brush_6[1][2] << " )";
	f << std::fixed << " ( " << brush_6[2][0] << " " << brush_6[2][1] << " " << brush_6[2][2] << " ) " << textures[(int)eTextures::posX] << " 64 64 0 0 0 0 lightmap_gray 16384 16384 0 0 0 0\n";

}
void CoD4::WriteLightGrid()
{
	brush_t brush;

	CellToBounds(&ui.vCells[0], brush);

	brush.mins[2] = 0;
	brush.maxs[2] = brushsize;

	std::array<const char*, 6> textures;

	textures[(int)eTextures::posZ] = "lightgrid_volume";
	textures[(int)eTextures::posY] = "lightgrid_volume";
	textures[(int)eTextures::posX] = "lightgrid_volume";
	textures[(int)eTextures::negX] = "lightgrid_volume";
	textures[(int)eTextures::negY] = "lightgrid_volume";
	textures[(int)eTextures::negZ] = "lightgrid_volume";

	WriteBrush(brush, ibrushIndex, textures);

}
void CoD4::WriteMapBarriers()
{

	brush_t firstBrush, lastBrush;

	CellToBounds(&ui.vCells[0], firstBrush);
	CellToBounds(&ui.vCells[ui.vCells.size()-1], lastBrush);

	vec3_t size;

	VectorSubtract(lastBrush.maxs, firstBrush.mins, size);

	brush_t brush;

	//-x, +y

	brush.mins[0] = firstBrush.mins[0] - brushsize;
	brush.mins[1] = firstBrush.mins[1];
	brush.mins[2] = 0;

	brush.maxs[0] = brush.mins[0] + brushsize;
	brush.maxs[1] = brush.mins[1] + size[1];
	brush.maxs[2] = brushsize;
	
	std::array<const char*, 6> textures;

	textures[(int)eTextures::posZ] = "clip";
	textures[(int)eTextures::posY] = "clip";
	textures[(int)eTextures::posX] = "clip";
	textures[(int)eTextures::negX] = "clip";
	textures[(int)eTextures::negY] = "clip";
	textures[(int)eTextures::negZ] = "clip";


	WriteBrush(brush, ibrushIndex, textures);

	//+x, +y

	brush.mins[0] = lastBrush.maxs[0];
	brush.mins[1] = firstBrush.mins[1];
	brush.mins[2] = 0;

	brush.maxs[0] = brush.mins[0] + brushsize;
	brush.maxs[1] = brush.mins[1] + size[1];
	brush.maxs[2] = brushsize;

	WriteBrush(brush, ibrushIndex, textures);

	//+x, -y

	brush.mins[0] = firstBrush.mins[0];
	brush.mins[1] = firstBrush.mins[1] - brushsize;
	brush.mins[2] = 0;

	brush.maxs[0] = brush.mins[0] + size[0];
	brush.maxs[1] = brush.mins[1] + brushsize;
	brush.maxs[2] = brushsize;

	WriteBrush(brush, ibrushIndex, textures);

	//+x, +y

	brush.mins[0] = firstBrush.mins[0];
	brush.mins[1] = lastBrush.maxs[1];
	brush.mins[2] = 0;

	brush.maxs[0] = brush.mins[0] + size[0];
	brush.maxs[1] = brush.mins[1] + brushsize;
	brush.maxs[2] = brushsize;

	WriteBrush(brush, ibrushIndex, textures);


}
void CoD4::WriteSkybox()
{
	brush_t firstBrush, lastBrush;

	CellToBounds(&ui.vCells[0], firstBrush);
	CellToBounds(&ui.vCells[ui.vCells.size() - 1], lastBrush);

	constexpr float MAZE_HEIGHT = 2048;

	std::array<const char*, 6> textures;

	textures[(int)eTextures::posZ] = "sky_armada";
	textures[(int)eTextures::posY] = "sky_armada";
	textures[(int)eTextures::posX] = "sky_armada";
	textures[(int)eTextures::negX] = "sky_armada";
	textures[(int)eTextures::negY] = "sky_armada";
	textures[(int)eTextures::negZ] = "sky_armada";

	vec3_t size;

	VectorSubtract(lastBrush.maxs, firstBrush.mins, size);

	brush_t brush;

	//skybox bottom

	brush.mins[0] = firstBrush.mins[0] - brushsize - 16;
	brush.mins[1] = firstBrush.mins[1] - brushsize - 16;
	brush.mins[2] = firstBrush.mins[2] - brushsize - 16;

	brush.maxs[0] = lastBrush.maxs[0] + brushsize + 16;
	brush.maxs[1] = lastBrush.maxs[1] + brushsize + 16;
	brush.maxs[2] = firstBrush.mins[2] - brushsize;

	WriteBrush(brush, ibrushIndex, textures);


	//skybox top

	brush.mins[0] = firstBrush.mins[0] - brushsize - 16;
	brush.mins[1] = firstBrush.mins[1] - brushsize - 16;
	brush.mins[2] = brushsize + MAZE_HEIGHT;

	brush.maxs[0] = lastBrush.maxs[0] + brushsize + 16;
	brush.maxs[1] = lastBrush.maxs[1] + brushsize + 16;
	brush.maxs[2] = brushsize + MAZE_HEIGHT + 16;

	WriteBrush(brush, ibrushIndex, textures);


	//UH
	brush.mins[0] = firstBrush.mins[0] - brushsize - 16;
	brush.mins[1] = firstBrush.mins[1] - brushsize - 16;
	brush.mins[2] = firstBrush.mins[2] - brushsize - 16;

	brush.maxs[0] = lastBrush.maxs[0] + brushsize + 16;
	brush.maxs[1] = brush.mins[1] + 16;
	brush.maxs[2] = brushsize + MAZE_HEIGHT + 16;

	WriteBrush(brush, ibrushIndex, textures);

	//UH
	brush.mins[0] = firstBrush.mins[0] - brushsize - 16;
	brush.mins[1] = firstBrush.mins[1] - brushsize - 16;
	brush.mins[2] = firstBrush.mins[2] - brushsize - 16;

	brush.maxs[0] = brush.mins[0] + 16;
	brush.maxs[1] = lastBrush.maxs[1] + brushsize + 16;
	brush.maxs[2] = brushsize + MAZE_HEIGHT + 16;

	WriteBrush(brush, ibrushIndex, textures);


	//+y, +x

	brush.mins[0] = firstBrush.mins[0] - brushsize;
	brush.mins[1] = lastBrush.maxs[1] + brushsize;
	brush.mins[2] = firstBrush.mins[2] - brushsize - 16;

	brush.maxs[0] = lastBrush.maxs[0] + brushsize + 16;
	brush.maxs[1] = brush.mins[1] + 16;
	brush.maxs[2] = brushsize + MAZE_HEIGHT + 16;


	WriteBrush(brush, ibrushIndex, textures);

	//+y, +x


	brush.mins[0] = lastBrush.maxs[0] + brushsize;
	brush.mins[1] = firstBrush.mins[1] - brushsize;
	brush.mins[2] = firstBrush.mins[2] - brushsize - 16;

	brush.maxs[0] = brush.mins[0] + 16;
	brush.maxs[1] = lastBrush.maxs[1] + brushsize + 16;
	brush.maxs[2] = brushsize + MAZE_HEIGHT + 16;



	WriteBrush(brush, ibrushIndex, textures);


}

void CoD4::WriteEntity(const vec3_t origin, const std::string_view& classname, vec3_t angles, const bool useAngles)
{
	f << "// entity " << ientityIndex << '\n';
	f << "{\n";

	f << "\"origin\" \"" << origin[0] << ' ' << origin[1] << ' ' << origin[2] << "\"\n";
	if (useAngles)
		f << "\"angles\" \"" << angles[0] << ' ' << angles[1] << ' ' << angles[2] << "\"\n";

	f << "\"classname\" \"" << classname << "\"\n";

	f << "}\n";

	ientityIndex++;
}
void CoD4::WriteEntityModel(const vec3_t origin, const std::string_view& classname, vec3_t angles, const bool useAngles)
{
	f << "// entity " << ientityIndex << '\n';
	f << "{\n";

	f << "\"origin\" \"" << origin[0] << ' ' << origin[1] << ' ' << origin[2] << "\"\n";
	if (useAngles)
		f << "\"angles\" \"" << angles[0] << ' ' << angles[1] << ' ' << angles[2] << "\"\n";

	f << "\"model\" \"" << classname << "\"\n";


	f << "\"classname\" \"" << "misc_model" << "\"\n";

	f << "}\n";

	ientityIndex++;
}
void CoD4::WriteEntityLight(const vec3_t origin, vec3_t color, float radius, float intensity)
{
	f << "// entity " << ientityIndex << '\n';
	f << "{\n";
	f << "\"origin\" \"" << origin[0] << ' ' << origin[1] << ' ' << origin[2] << "\"\n";
	f << "\"def\" \"light_point_linear" << "" << "\"\n";
	f << "\"radius\" \"" << radius << "\"\n";
	f << "\"_color\" \"" << color[0] << ' ' << color[1] << ' ' << color[2] << "\"\n";
	f << "\"intensity\" \"" << intensity << "\"\n";
	f << "\"classname\" \"" << "light" << "\"\n";
	f << "}\n";

	ientityIndex++;
}
void CoD4::WriteTrigger(const brush_t& brush, const std::string_view& hintstring)
{
	f << "// entity " << ientityIndex << '\n';
	f << "{\n";


	f << "\"hintstring\" \"" << hintstring << "\"\n";

	f << "\"classname\" \"" << "trigger_use_touch" << "\"\n";

	std::array<const char*, 6> textures = {"trigger" ,"trigger", "trigger", "trigger", "trigger", "trigger"};

	WriteBrush(brush, 0, textures);

	f << "}\n";
	ientityIndex++;


}
void CoD4::WriteEntities()
{

	brush_t firstBrush, triggerBrush;

	CellToBounds(&ui.vCells[0], firstBrush);

	vec3_t origin, angles;
	vec3_t trig_mins, trig_maxs;



	VectorCopy(firstBrush.mins, triggerBrush.mins);
	VectorCopy(firstBrush.maxs, triggerBrush.maxs);

	triggerBrush.mins[2] = triggerBrush.maxs[2];
	triggerBrush.maxs[2] = triggerBrush.mins[2] + 16;

	char buff[256];

	sprintf_s(buff, "This is an auto-generated maze of size ^2%dx%d^7, blocksize: ^2%.1f ^7units! The end platform is marked with a black and red texture", ui.iPixelsPerAxis.x, ui.iPixelsPerAxis.y, brushsize);

	std::string hintstring = buff;

	WriteTrigger(triggerBrush, hintstring);


	CellToBounds(&ui.vCells[ui.vCells.size() -1 ], triggerBrush);

	triggerBrush.mins[2] = triggerBrush.maxs[2];
	triggerBrush.maxs[2] = triggerBrush.mins[2] + 16;

	sprintf_s(buff, "Congratulations! You travelled approximately ^2%.1f ^7units to get here", sqrt(firstBrush.origin[0] * firstBrush.origin[0] + triggerBrush.origin[0] * triggerBrush.origin[1]));

	hintstring = buff;

	WriteTrigger(triggerBrush, hintstring);



	origin[0] = firstBrush.origin[0];
	origin[1] = firstBrush.origin[1];
	origin[2] = 24;

	angles[0] = 0;
	angles[1] = 0;
	angles[2] = 0;

	WriteEntity(origin, "mp_dm_spawn", angles, true);

	origin[2] += 96;

	WriteEntity(origin, "mp_global_intermission");

	origin[2] += 24;

	WriteEntity(origin, "reflection_probe");

	auto begin = ui.vCells.begin();
	auto end = ui.vCells.end();


	//spawn farmers to each deadend
	//spawn a red light above them
	if (EXPORT_DEADEND_STUFF) {
		for (auto it = begin; it != end; ++it) {

			if (!it->bDeadend)
				continue;


			brush_t brush;
			CellToBounds(&*it, brush);


			WriteEntityModel(brush.origin, "character_russian_farmer", vec3_t{ 0, it->fDeadendAngle, 0 }, true);
			brush.origin[2] += 80;
			WriteEntityLight(brush.origin, vec3_t{ 1,0,0 }, 300, 1.f);
		}
	}


}
void CoD4::BeginConversion()
{
	//if (!mazedata) {
	//	FatalError("BeginConversion(): called with invalid maze data");
	//	return;
	//}

	if (!fs::F_OpenFile(f, path, fs::fileopen::FILE_OUT)) {
		FatalError("CoD4::BeginConversion(): can't open file for OUT operation! Error: ", fs::_GetLastError());
		return;
	}

	WriteMapRelatedUnnecessaryStuff();

	brush_t brush;

	ibrushIndex = 0;
	ientityIndex = 1;

	std::array<const char*, 6> textures;

	textures[(int)eTextures::negZ] = "caulk";

	auto begin = ui.vCells.begin();
	auto end = ui.vCells.end();


	int i = 0;
	for (auto it = begin; it != end; ++it) {


		Maze::sCell* cell = &*it;

		if (it == begin || i == ui.vCells.size()-1)
			CellToBounds(cell, brush);
		else
			CellsToBounds(cell, brush, it);

		if (brush.bWalkable) {
			textures[(int)eTextures::posX] = "caulk";
			textures[(int)eTextures::posY] = "caulk";
			textures[(int)eTextures::posZ] = "ch_brick_wall_08";
			textures[(int)eTextures::negX] = "caulk";
			textures[(int)eTextures::negY] = "caulk";

		}
		else {
			textures[(int)eTextures::posX] = "ch_brick_wall_07";
			textures[(int)eTextures::posY] = "ch_brick_wall_07";
			textures[(int)eTextures::posZ] = "ch_brick_wall_07";
			textures[(int)eTextures::negX] = "ch_brick_wall_07";
			textures[(int)eTextures::negY] = "ch_brick_wall_07";
		}

		if (ibrushIndex == 0) {
			textures[(int)eTextures::posZ] = "icbm_hazardstripe2";
		}
		else if (ibrushIndex == ui.vCells.size() - 1 || i == ui.vCells.size() - 1) {
			textures[(int)eTextures::posZ] = "icbm_freightelevator";

		}

		i++;
		WriteBrush(brush, ibrushIndex, textures);
	}

	Maze::sCell* cell = &ui.vCells.back();
	CellToBounds(cell, brush);

	textures[(int)eTextures::posX] = "caulk";
	textures[(int)eTextures::posY] = "caulk";
	textures[(int)eTextures::posZ] = "icbm_freightelevator";
	textures[(int)eTextures::negX] = "caulk";
	textures[(int)eTextures::negY] = "caulk";

	WriteBrush(brush, ibrushIndex, textures);


	WriteLightGrid();
	WriteMapBarriers();
	WriteSkybox();

	//end brushes
	f << "}\n";

	WriteEntities();

	fs::F_CloseFile(f);
}