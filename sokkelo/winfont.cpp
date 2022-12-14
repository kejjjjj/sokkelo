
#include "sokkelo.h"

bool Font_s::InsertFont(const char* name, std::string fontpath, float scale)
{
	ImGuiIO* io = &ImGui::GetIO();

	if (!io) {
		FatalError("Font_s::InsertFont(): ImGui::GetIO() returned 0");
		return false;
	}
	fonts.push_back(std::make_pair(io->Fonts->AddFontFromFileTTF(fontpath.c_str(), scale), name));

	return true;
}
ImFont* Font_s::FetchFont(const char* name)
{
	for (const auto& i : fonts) {
		if (!strcmp(name, i.second))
			return i.first;
	}

	return nullptr;
}
void Font_s::LoadHardcodedFonts()
{
	InsertFont("PixelatedM", (root_directory + "\\fonts\\pixelated.ttf"), 40);
	InsertFont("PixelatedSM", (root_directory + "\\fonts\\pixelated.ttf"), 20);
	InsertFont("PixelatedL", (root_directory + "\\fonts\\pixelated.ttf"), 50);
	InsertFont("PixelatedXL", (root_directory + "\\fonts\\pixelated.ttf"), 60);

}