#pragma once

#ifndef mndwnd
#define mndwnd
#include "sokkelo.h"

namespace MW
{

	void RenderTabBar();
	void TabBarAction(const char* text, std::function<void()> a);
	void ButtonAction(const char* text, std::function<void()> a);
	void ToggleAction(const char* text, bool& var, std::function<void()> a);


}
#endif