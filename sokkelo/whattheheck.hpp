#pragma once

//PURPOSE:
//converts the finished maze map into a Call of Duty 4 Radiant .map file
//yes...

#ifndef cod4rad
#define cod4rad

#include "sokkelo.h"

struct CoD4
{


	struct brush_t
	{
		vec3_t origin;
		vec3_t mins;
		vec3_t maxs;
		bool bWalkable;
	};

	enum class eTextures
	{
		posZ,
		posX,
		posY,
		negX,
		negY,
		negZ,
		INVALID

	};

	CoD4(const std::string_view& dir, const Maze* _mazedata, const float _brushsize = 512) : 
		path(dir), 
		mazedata(_mazedata),
		brushsize(_brushsize) {};

	~CoD4() { fs::F_CloseFile(f); }

	void BeginConversion();
	void WriteMapRelatedUnnecessaryStuff();
	void WriteLightGrid();
	void WriteMapBarriers();
	void WriteSkybox();
	void WriteEntities();
	void WriteEntity(const vec3_t origin, const std::string_view& classname, vec3_t angles = {}, const bool useAngles = false);
	void WriteEntityModel(const vec3_t origin, const std::string_view& classname, vec3_t angles = {}, const bool useAngles = false);
	void WriteEntityLight(const vec3_t origin, vec3_t color, float radius = 200, float intensity = 1);
	void WriteTrigger(const brush_t& brush, const std::string_view& hintstring);

	void CellToBounds(const Maze::sCell* cell, brush_t& brush);
	void CellsToBounds(Maze::sCell* cell, brush_t& brush, std::vector<Maze::sCell>::iterator& it);

	void WriteBrush(const brush_t& brush, const int brushIndex, std::array<const char*, 6>& textures);
	void WriteBrushContents(const brush_t& brush, std::array<const char*, 6>& textures);


private:
	std::string_view path;
	std::fstream f;
	const Maze* mazedata;
	float brushsize = 512;
	size_t ibrushIndex;
	size_t ientityIndex;
};


#endif
