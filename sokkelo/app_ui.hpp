#pragma once

#ifndef appui
#define appui

#include "sokkelo.h"

class AppUI
{
public:
	bool Construct(const ImVec2& a_mins, const float& w, const float& h, const float& gridSize = -1, const float& padding = 0.f, const bool& border = true, const ImVec4& bg_col = COL::WHITE, const ImVec4& tile_col = COL::BLACK, const ImVec4 _borderCol = COL::WHITE) {
		return Construct(a_mins, ImVec2(w, h), gridSize, padding, border, bg_col, tile_col, borderCol);
	}


	bool Construct(const ImVec2& a_mins, const ImVec2& a_maxs, const float& gridSize = -1, const float& padding = 0.f, const bool& border = true, const ImVec4& bg_col = COL::WHITE, const ImVec4& tile_col = COL::BLACK, const ImVec4 _borderCol = COL::WHITE);

	bool IsConstructed() { return bIsConstructed; }
	void Kill() { bIsConstructed = false; }

	void Render();


	void DrawRectangleFilled(const ImVec2& Pixel, const ImVec4& col) {
		size_t offsetX = (real_maxs.x - real_mins.x) / (uPixels);
		size_t offsetY = (real_maxs.y - real_mins.y) / (uPixels);

		gui::DrawRectangleFilled(ImVec2(real_mins.x + Pixel.x, real_mins.y + Pixel.y), offsetX, offsetY, col);
		gui::DrawRectangleFilled(ImVec2(real_mins.x + offsetX, real_mins.y + offsetY), offsetX, offsetY, COL::RED);

	}


private:

	ImVec2 mins, maxs;
	ImVec2 real_mins, real_maxs;
	float fWidth, fHeight;
	float fGridSize = -1.f;
	float fTimeElapsed = 0.f; 
	float fPadding = 0.f;
	bool bBorder = true;
	ImVec4 bgCol;
	ImVec4 tileCol;
	ImVec4 borderCol;
	DWORD dwTimeCreated = 0;

	bool bIsConstructed = 0;

	int32_t uPixels;
};

inline AppUI ui;

#endif