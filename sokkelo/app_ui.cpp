#include "sokkelo.h"

bool AppUI::Construct(const ImVec2& a_mins, const ImVec2& a_maxs, const float& gridSize,  const bool& border, const ImVec4& bg_col, const ImVec4& tile_col, const ImVec4 _borderCol) {

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

	real_mins = ImVec2(mins.x, mins.y);
	real_maxs = ImVec2(maxs.x - 16, maxs.y - 36);

	bBorder = border;

	int x_pixel_count = real_maxs.x - real_mins.x;
	int y_pixel_count = real_maxs.y - real_mins.y;

	x_pixel_count /= fGridSize;
	y_pixel_count /= fGridSize;

	fAspectRatio = float(x_pixel_count) / float(y_pixel_count);


	uPixels = (int)(x_pixel_count + y_pixel_count) / (fGridSize);

	if (uPixels <= 0)		uPixels = 1;
	if (x_pixel_count <= 0)	x_pixel_count = 1;
	if (y_pixel_count <= 0)	y_pixel_count = 1;

	vPixelSize = ImVec2((x_pixel_count + y_pixel_count) / x_pixel_count * (fGridSize / 2), 
						(x_pixel_count + y_pixel_count) / y_pixel_count * (fGridSize / 2));

	vPixelSize.x = (int)vPixelSize.x;
	vPixelSize.y = (int)vPixelSize.y;

	if (vPixelSize.x > vPixelSize.y)
		vPixelSize.y = vPixelSize.x;
	else
		vPixelSize.x = vPixelSize.y;

	
	iPixelsPerAxis.x = x_pixel_count;
	iPixelsPerAxis.y = y_pixel_count;

	real_maxs.x = real_mins.x + ((iPixelsPerAxis.x) * vPixelSize.x);
	real_maxs.y = real_mins.y + ((iPixelsPerAxis.y) * vPixelSize.y);

	uPixels *= uPixels;

	return true;
}
void AppUI::Render()
{

	if (!bIsConstructed)
		return;

	if (uPixels < 0) {
		FatalError("Invalid rectangle!");
		return;
	}

	ImGui::NewLine();

	if (ui.bThreadActive) { ImGui::BeginDisabled(); }


	if (ButtonCentered("Generate", 0.45f)) {
		ui.StartGeneration();
	}

	if (ui.bThreadActive) { ImGui::EndDisabled(); }


	if (!ui.bFinished) {ImGui::BeginDisabled();}

	ImGui::SameLine();

	if (ImGui::Button("Solve")) {
		if (!solution.Initialized() && ui.bFinished) {
			solution.OnExit();
			solution.Initialize();
		}
		std::thread(solution.DoStuffWrapper).detach();
		Sleep(100);
	}

	if (!ui.bFinished) {ImGui::EndDisabled();}


	ImGui::Text("Size (%i, %i) - Algorithm: %s", iPixelsPerAxis.x, iPixelsPerAxis.y, ui.GetAlgorithm() == Maze::eMazeAlgorithm::Depth_First ? "Depth-first" : "Aldous-Broder");

	Clear(COL::BLACK);

	if (!ui.vCells.empty() && ui.bAbleToRender) {

		size_t i = 0;
		for (auto& cell : ui.vCells) {

			if (cell.bPathLeadsToDeadend && SOLUTION_DRAW_DEADENDS) {
				Draw(cell.vPos, COL::RED);

			}
			else if (cell.bVisited) {
				Draw(cell.vPos, COL::WHITE);
			}
			else if (cell.bWall) {
				Draw(cell.vPos, COL::BLACK);

			}
			i++;
		}
		if (solution.Initialized()) {
			Draw(solution.sCurrentTile->vPos, ImVec4(255, 0, 255, 255));

		}
		if (solution.Finished()) {
			for (const auto& i : solution.vFinishPath) {
				Draw(i->vPos, ImVec4(0, 255, 0, 255));
			}
		}
		const Maze::sCell* last = &ui.vCells.back();
		if(last->bVisited)
			Draw(last->vPos, COL::GREEN);
	}


	if (bBorder) {

		//all the 1 means gridSize


		//left
		gui::DrawRectangleFilled(ImVec2(mins.x, mins.y), ImVec2(mins.x + 1, real_maxs.y), borderCol);


		//top
		gui::DrawRectangleFilled(ImVec2(mins.x, mins.y), ImVec2(real_maxs.x, mins.y + 1), borderCol);

		//right
		gui::DrawRectangleFilled(ImVec2(real_maxs.x - 1, mins.y),	real_maxs, borderCol);

		//bottom
		gui::DrawRectangleFilled(ImVec2(mins.x, real_maxs.y - 1),	real_maxs, borderCol);
	}

	ImGui::Dummy(ImVec2(real_maxs.x - real_mins.x, real_maxs.y - real_mins.y + 10));

	ImGui::Text("Generations: %u", solution.iGenerations);

}