#pragma once

#ifndef appui
#define appui

#include "sokkelo.h"

class AppUI
{
public:
	bool Construct(const ImVec2& a_mins, const float& w, const float& h, const float& gridSize = -1, const float& padding = 0.f, const bool& border = true, const ImVec4& bg_col = COL::WHITE, const ImVec4& tile_col = COL::BLACK, const ImVec4 _borderCol = COL::WHITE) {
		return Construct(a_mins, ImVec2(w, h), gridSize, padding, border, bg_col, tile_col, _borderCol);
	}


	bool Construct(const ImVec2& a_mins, const ImVec2& a_maxs, const float& gridSize = -1, const float& padding = 0.f, const bool& border = true, const ImVec4& bg_col = COL::WHITE, const ImVec4& tile_col = COL::BLACK, const ImVec4 _borderCol = COL::WHITE);

	bool IsConstructed() { return bIsConstructed; }
	void Kill() { bIsConstructed = false; }

	void Render();


	void Draw(const ImVec2& Pixel, const ImVec4& col) {
		return gui::DrawRectangleFilled(

			ImVec2(real_mins.x + (Pixel.x * vPixelSize.x),					real_mins.y + Pixel.y * vPixelSize.y), //mins
			ImVec2(real_mins.x + (Pixel.x * vPixelSize.x) + vPixelSize.x,	real_mins.y + (Pixel.y * vPixelSize.y) + vPixelSize.y), col); //maxs
	}
	void Clear(const ImVec4& col) { gui::DrawRectangleFilled(real_mins, real_maxs, col); };

	ImVec2 GetWindowPos() const { return real_mins; }
	ImVec2 GetWindowSize() const { return ImVec2(real_maxs.x - real_mins.x, real_maxs.y - real_mins.y); }


	bool bIsConstructed = 0;

	int32_t uPixels = 0;
	int32_t iPixelsPerAxis = 0;
	float fAspectRatio = 0;
	
private:

	ImVec2 mins, maxs;
	ImVec2 real_mins, real_maxs;
	float fWidth = 0, fHeight = 0;
	float fGridSize = -1.f;
	float fTimeElapsed = 0.f; 
	float fPadding = 0.f;
	bool bBorder = true;
	ImVec4 bgCol;
	ImVec4 tileCol;
	ImVec4 borderCol;
	DWORD dwTimeCreated = 0;
	ImVec2 vPixelSize;

};

//inline AppUI ui;

#endif