#pragma once

#ifndef cmnshapes
#define cmnshapes

#include "sokkelo.h"

namespace gui
{
	void DrawRectangle(const ImVec2& mins, const ImVec2& maxs, const ImVec4& col);
	void DrawRectangle(const ImVec2& mins, const float& w, const float& h, const ImVec4& col);
	void DrawRectangleFilled(const ImVec2& mins, const ImVec2& maxs, const ImVec4& col);
	void DrawRectangleFilled(const ImVec2& mins, const float& w, const float& h, const ImVec4& col);

	void DrawCircle(const ImVec2& pos, const float& radius, const ImVec4& col);
	void DrawCircleFilled(const ImVec2& pos, const float& radius, const ImVec4& col);

}


#endif