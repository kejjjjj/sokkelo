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

	CoD4(const std::string_view& dir, const Maze* _mazedata, const float _brushsize = 512) : 
		path(dir), 
		mazedata(_mazedata),
		brushsize(_brushsize) {};

	~CoD4() { fs::F_CloseFile(f); }

	void BeginConversion();
	void WriteMapRelatedUnnecessaryStuff();
	void CellToBounds(const Maze::sCell* cell, brush_t& brush);
	void WriteBrush(const brush_t& brush, const int brushIndex);

private:
	std::string_view path;
	std::fstream f;
	const Maze* mazedata;
	float brushsize = 512;
};


#endif
