#pragma once

#ifndef mndwnd
#define mndwnd
#include "sokkelo.h"

inline bool UPDATE_VIEWPORT = false;
inline float VIEWPORT_GRID = 40;
inline ImVec2 VIEWPORT_SIZE(1280, 720);

namespace MW
{

	void RenderTabBar();
	void TabBarAction(const char* text, std::function<void()> a);
	void ButtonAction(const char* text, std::function<void()> a);
	void ToggleAction(const char* text, bool& var, std::function<void()> a);

	void RenderExportSettings();
}
#endif