
#pragma once

#ifndef basewind
#define basewind

#include "sokkelo.h"

inline std::vector<std::string> compilerlog;
inline void FatalError()
{

	std::string beg /*= std::format("Line: {}\nColumn: {}\n\nDescription:\n", fs::file.lines_read, fs::file.current_column)*/;

	//compilerlog.insert(compilerlog.begin(), beg);

	beg.clear();
	for (const auto& i : compilerlog) {
		beg += i;
	}

	MessageBoxA(NULL, beg.c_str(), "Compile Error!", MB_ICONERROR);
	compilerlog.clear();
	exit(-1);
	return;
}

template <typename T, typename ... Param>
inline void FatalError(const T& t, const Param& ... param)
{
	std::stringstream ss;
	ss << t;
	compilerlog.push_back(ss.str());

	//std::cout << t << '\n';
	FatalError(param...);

}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class MainWindow
{
public:

    MainWindow() {};
    ~MainWindow() {};

    bool InitializeWindow(const char* p_szTitle, HINSTANCE hInst, WNDPROC _wndProc);
    void ExitApplication();
    void KillWindow();
    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void CleanupRenderTarget();
    void CreateRenderTarget();
    void CleanupDeviceD3D();
    HWND GetWindowHandle() { return hwnd; }
    bool Render(ImGuiIO& io, MSG& msg);

    AppWindow window;


    ID3D11Device* g_pd3dDevice = NULL;
    ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
    IDXGISwapChain* g_pSwapChain = NULL;
    ID3D11RenderTargetView* g_mainRenderTargetView = NULL;


private:
    WNDCLASSEXA wc;
    HWND hwnd = 0;
    std::string szTitle = "";
    bool initialized = FALSE;
};

inline MainWindow hWnd_main;

#endif