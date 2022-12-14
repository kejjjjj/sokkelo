#pragma once

#ifndef winfont
#define winfont

#include "sokkelo.h"

inline std::string root_directory;

enum FontEnum
{
	Pixelated_SM,
	Pixelated_M,
	Pixelated_L,
	Pixelated_XL


};

struct Font_s
{

	Font_s() : fonts(NULL) {};

	~Font_s(){ fonts.clear();}

	bool InsertFont(const char* name, std::string fontpath, float scale);
	ImFont* FetchFont(const char* name);


	std::vector< std::pair<ImFont*, const char*> > fonts;
	void LoadHardcodedFonts();

};


#endif