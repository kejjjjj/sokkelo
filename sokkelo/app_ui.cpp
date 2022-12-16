#include "sokkelo.h"

bool AppUI::Construct(const ImVec2& a_mins, const ImVec2& a_maxs, const float& gridSize, const float& padding,  const bool& border, const ImVec4& bg_col, const ImVec4& tile_col, const ImVec4 _borderCol) {

	//if (bIsConstructed)
	//	return true;

	if (!ImGui::GetCurrentContext()) {
		FatalError("AppUI::Construct failed with: ImGui::GetCurrentContext() == 0");
		return false;
	}

	//if (ImGui::GetCurrentContext()->CurrentWindow) {
	//	const ImVec2 wnd_pos = ImGui::GetWindowPos();
	//	const ImVec2 wnd_size = ImGui::GetWindowSize();

	//	if (a_mins.x < wnd_pos.x || a_mins.y < wnd_pos.y || (wnd_pos.x + wnd_size.x) < a_maxs.x || (wnd_pos.y + wnd_size.y) < a_maxs.y) {

	//		//FatalError("AppUI::Construct failed with: Set bounds are not within the window bounds");

	//		return false;
	//	}
	//}

	dwTimeCreated = Sys_MilliSeconds();
	fGridSize = gridSize < 1 ? 8 : gridSize;
	bgCol = bg_col;
	tileCol = tile_col;
	borderCol = _borderCol;

	mins = a_mins;
	maxs = a_maxs;
	bIsConstructed = true;

	fWidth = maxs.x - mins.x;
	fHeight = maxs.y - mins.y;

	fPadding = padding;

	real_mins = ImVec2(mins.x + fPadding, mins.y + fPadding);
	real_maxs = ImVec2(maxs.x - fPadding - 16, maxs.y - fPadding - 36);

	bBorder = border;

	fAspectRatio = float(real_maxs.x - real_mins.x) / float(real_maxs.y - real_mins.y);

	int x_pixel_count = real_maxs.x - real_mins.x;
	int y_pixel_count = real_maxs.y - real_mins.y;

	uPixels = (int)(x_pixel_count + y_pixel_count) / (fGridSize);

	vPixelSize = ImVec2(x_pixel_count / uPixels, y_pixel_count / uPixels);
	iPixelsPerAxis = uPixels;
	uPixels *= uPixels;

	return true;
}
void AppUI::Render()
{

	if (!bIsConstructed)
		return;


	//int x_pixel_count = real_maxs.x - real_mins.x;
	//int y_pixel_count = real_maxs.y - real_mins.y;

	//uPixels = (int)(x_pixel_count + y_pixel_count) / (fGridSize);

	if (uPixels < 0) {
		FatalError("Invalid rectangle!");
		return;
	}





	int horzPixels = iPixelsPerAxis;
	int vertPixels = iPixelsPerAxis;

	//uPixels *= uPixels;

	//ImVec2 Pixel = ImVec2(2, 0);
	//
	//Draw(Pixel, COL::RED);
	//for (int y = 0; y < vertPixels; y++) {

	//	for (int x = 0; x < horzPixels; x++) {

	//		Draw(ImVec2(x, y), ImVec4(255, 255, 255, x % 2 == 0 ? 255 : 0));
	//	}

	//}
	
	if (GetAsyncKeyState(VK_NUMPAD8) & 1) {

		ui.generation_thread = std::thread(ui.IterativeGenerationWrapper, 0);

	}
	if (GetAsyncKeyState(VK_NUMPAD9) & 1) {

		ui.generation_thread = std::thread(ui.AldousBroderAlgorithmWrapper, 0);


	}
	ImGui::Text("Pixels (%i, %i)\naspectRatio: %.2f", iPixelsPerAxis, iPixelsPerAxis, fAspectRatio);

	Clear(COL::BLACK);

	if (!ui.vCells.empty() && ui.bAbleToRender) {

		size_t i = 0;
		for (auto& cell : ui.vCells) {

			if (cell.bBacktraced) {
				Draw(cell.vPos, COL::BLUE);

			}
			else if (cell.bVisited) {
				Draw(cell.vPos, COL::WHITE);
			}
			else if (cell.bWall) {
				Draw(cell.vPos, COL::BLACK);

			}
			//else {
			//	Draw(cell.vPos, COL::WHITE);
			//}
			i++;
		}
		Maze::sCell* last = &ui.vCells.back();
		if(last->bVisited)
			Draw(last->vPos, COL::GREEN);
	}


	if (bBorder) {

		const float Width = (fWidth - fPadding * 2) - 1 - 16;
		const float Height = (fHeight - fPadding * 2) - 36;

		//all the 1 means gridSize

		//left
		gui::DrawRectangleFilled(ImVec2(mins.x + fPadding, mins.y + fPadding), 1, Height, borderCol);

		//top
		gui::DrawRectangleFilled(ImVec2(mins.x + fPadding, mins.y + fPadding), Width, 1, borderCol);


		//right
		gui::DrawRectangleFilled(ImVec2(maxs.x - 1 - fPadding - 16, mins.y + fPadding),	real_maxs, borderCol);

		//bottom
		gui::DrawRectangleFilled(ImVec2(mins.x + fPadding, maxs.y - fPadding - 36 - 1),	real_maxs, borderCol);
	}

}