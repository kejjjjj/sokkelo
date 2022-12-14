#pragma once
#ifndef appwin
#define appwin
#include "sokkelo.h"


class AppWindow
{
public:

	AppWindow() : Size(), Pos(), open(true), active() {}
	//void CornerActionButtons();

	ImVec2 Size;
	ImVec2 Pos;
	bool active;
	bool open;

private:
};

#endif