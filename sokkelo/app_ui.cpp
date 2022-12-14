#include "sokkelo.h"

bool AppUI::Construct(const ImVec2& a_mins, const ImVec2& a_maxs, const float& gridSize, const float& padding,  const bool& border, const ImVec4& bg_col, const ImVec4& tile_col, const ImVec4 _borderCol) {

	//if (bIsConstructed)
	//	return true;

	if (!ImGui::GetCurrentContext()) {
		FatalError("AppUI::Construct failed with: ImGui::GetCurrentContext() == false");
		return false;
	}

	if (ImGui::GetCurrentContext()->CurrentWindow) {
		const ImVec2 wnd_pos = ImGui::GetWindowPos();
		const ImVec2 wnd_size = ImGui::GetWindowSize();

		if (a_mins.x < wnd_pos.x || a_mins.y < wnd_pos.y || (wnd_pos.x + wnd_size.x) < a_maxs.x || (wnd_pos.y + wnd_size.y) < a_maxs.y) {

			//FatalError("AppUI::Construct failed with: Set bounds are not within the window bounds");

			return false;
		}
	}

	dwTimeCreated = Sys_MilliSeconds();
	fGridSize = gridSize < 1 ? 8 : gridSize;
	bgCol = bg_col;
	tileCol = tile_col;
	borderCol = _borderCol;

	mins = a_mins;
	maxs = ImVec2(a_maxs.x, a_maxs.y);
	bIsConstructed = true;

	fWidth = maxs.x - mins.x;
	fHeight = maxs.y - mins.y;

	fPadding = padding;

	uPixels = (fWidth * fHeight) / gridSize;

	real_mins = ImVec2(mins.x + fPadding, mins.x + fPadding);
	real_maxs = ImVec2(maxs.x - fPadding - 16, maxs.y - fPadding - 36);

	bBorder = border;

	return true;
}
void AppUI::Render()
{

	if (!bIsConstructed)
		return;


	if (bBorder) {

		//all the 1 means gridSize

		//left
		gui::DrawRectangleFilled(ImVec2(mins.x + fPadding, mins.y + fPadding), 1, (fHeight - fPadding * 2) - 36, borderCol);

		//top
		gui::DrawRectangleFilled(ImVec2(mins.x + fPadding, mins.x + fPadding), (fWidth - fPadding * 2) - 1 - 16, 1, borderCol);

		//right
		gui::DrawRectangleFilled(ImVec2(maxs.x - 1 - fPadding - 16, mins.y + fPadding), ImVec2(maxs.x - fPadding - 16, fHeight - fPadding - 36), borderCol);

		//bottom
		gui::DrawRectangleFilled(ImVec2(mins.x + fPadding, maxs.y - fPadding - 36 - 1), ImVec2(maxs.x - fPadding - 16, maxs.y - fPadding - 36), borderCol);
	}

	int x_pixel_count = real_maxs.x - real_mins.x;
	int y_pixel_count = real_maxs.y - real_mins.y;

	uPixels = (int)((x_pixel_count / ((int)fGridSize * 2)) * (y_pixel_count / ((int)fGridSize * 2)));
	if (uPixels < 0) {
		FatalError("Invalid rectangle!");
		return;
	}

//	uPixels *= uPixels;
	DrawRectangleFilled(ImVec2(1, 1), COL::GREEN);



	ImGui::Text("Total Pixels: %i", uPixels * uPixels);

}