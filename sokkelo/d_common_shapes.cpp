#include "sokkelo.h"

void gui::Draw(const ImVec2& pos, const ImVec4& col)
{
	ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + 1, pos.y + 1), IM_COL32(col.x, col.y, col.z, col.w));

}
void gui::DrawRectangle(const ImVec2& mins, const ImVec2& maxs, const ImVec4& col)
{
	ImGui::GetWindowDrawList()->AddRect(mins, maxs, IM_COL32(col.x, col.y, col.z, col.w));
}
void gui::DrawRectangle(const ImVec2& mins, const float& w, const float& h, const ImVec4& col)
{
	ImGui::GetWindowDrawList()->AddRect(mins, ImVec2(mins.x + w, mins.y + h), IM_COL32(col.x, col.y, col.z, col.w));
}
void gui::DrawRectangleFilled(const ImVec2& mins, const ImVec2& maxs, const ImVec4& col)
{
	ImGui::GetWindowDrawList()->AddRectFilled(mins, maxs, IM_COL32(col.x, col.y, col.z, col.w));
}
void gui::DrawRectangleFilled(const ImVec2& mins, const float& w, const float& h, const ImVec4& col)
{
	ImGui::GetWindowDrawList()->AddRectFilled(mins, ImVec2(mins.x + w, mins.y + h), IM_COL32(col.x, col.y, col.z, col.w));
}
void gui::DrawCircle(const ImVec2& pos, const float& radius, const ImVec4& col)
{
	ImGui::GetWindowDrawList()->AddCircle(pos, radius, IM_COL32(col.x, col.y, col.z, col.w));
}
void gui::DrawCircleFilled(const ImVec2& pos, const float& radius, const ImVec4& col)
{
	ImGui::GetWindowDrawList()->AddCircleFilled(pos, radius, IM_COL32(col.x, col.y, col.z, col.w));
}