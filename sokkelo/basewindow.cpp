#include "sokkelo.h"

bool MainWindow::InitializeWindow(const char* p_szTitle, HINSTANCE hInst, WNDPROC _wndProc)
{
    //  wndProc = _wndProc;
     // WNDCLASSEXA wc;

    wc.cbSize = (sizeof(WNDCLASSEXA));
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = _wndProc;
    wc.cbClsExtra = 0l;
    wc.cbWndExtra = 0l;
    wc.hInstance = hInst;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0;
    wc.lpszClassName = p_szTitle;
    wc.hIconSm = 0;

    //RenderFunctions.clear();
    //window.open = true;
    // szTitle = std::string(p_szTitle);


    ::RegisterClassExA(&wc);

    hwnd = ::CreateWindowA(wc.lpszClassName, p_szTitle, WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    if (!hwnd) {

        FatalError("::CreateWindowA(wc.lpszClassName, p_szTitle, WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL)\nreturned NULL");
        return false;
    }

    const auto CreateDevice = [this]()-> bool {

        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK) {
            FatalError("(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)");
            return false;
        }

        CreateRenderTarget();
        return true;

    };
    if (!CreateDevice()) {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return false;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    // Setup Platform/Renderer backends
    if (!ImGui_ImplWin32_Init(hwnd)) {
        FatalError("!ImGui_ImplWin32_Init(hwnd)");
        return false;
    }

    if (!ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext)) {
        FatalError("!ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext)");
        return false;
    }

    sys_baseTime = timeGetTime();
    root_directory = fs::GetRootDirectory();

    

    rsc.LoadAllResources();
    // MessageBoxA(NULL, fs::GetRootDirectory().c_str(), "Root Directory", MB_ICONEXCLAMATION);
    initialized = true;
    return true;

}
void MainWindow::KillWindow()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}
void MainWindow::ExitApplication()
{
    //rsc.Font.~Font_s();
    KillWindow();

    exit(-1);

}
void MainWindow::CleanupRenderTarget()
{
    if (g_mainRenderTargetView != nullptr) {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = NULL;
    }
}
void MainWindow::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}
void MainWindow::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}
LRESULT WINAPI MainWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    MainWindow* wnd = &hWnd_main;

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:

        if (wnd->g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            if (wnd->initialized == 1) {
                wnd->CleanupRenderTarget();
                wnd->g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                wnd->CreateRenderTarget();
                tagRECT rect;
                GetWindowRect(wnd->hwnd, &rect);

                int height = rect.bottom - rect.top;
                int width = rect.right - rect.left;

                wnd->window.Size = ImVec2(width, height);

               // ui.Construct(ImVec2(hWnd_main.window.Pos), ImVec2(hWnd_main.window.Size), 1, 32);

                //SetWindowPos(hWnd_main.GetWindowHandle(), NULL, rect.left, rect.top, width, height, 0);
                ui.bIsConstructed = false;
                solution.OnExit();
                ui.KillGeneration();

            }
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        //wnd->ExitApplication();
        ::PostQuitMessage(0);
        //exit(-1);
        return 0;
    case WM_DPICHANGED:
        //if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        //{
        //    const RECT* suggested_rect = (RECT*)lParam;
        //    ::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        //}
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
bool MainWindow::Render(ImGuiIO& io, MSG& msg)
{

    if (!ImGui::GetCurrentContext())
        return false;

    static const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin(wc.lpszClassName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

    MW::RenderTabBar();

    if (DRAW_EXPORT_WINDOW) {
        MW::RenderExportSettings();
    }
    else {
        static float gridsize = 106.39f;

        if (!ui.IsConstructed() || UPDATE_VIEWPORT) {
            solution.OnExit();
            ui.KillGeneration();
            ui.Construct(ImVec2(hWnd_main.window.Pos.x + 10, hWnd_main.window.Pos.y + 200), VIEWPORT_SIZE.x, VIEWPORT_SIZE.y, VIEWPORT_GRID, true);
            UPDATE_VIEWPORT = false;
        }
        ui.Render();
    }
    window.active = ImGui::IsWindowFocused();

    ImGui::SetWindowSize(hWnd_main.window.Size);
    ImGui::SetWindowPos(hWnd_main.window.Pos);

    ImGui::End();



    // Rendering
    ImGui::Render();
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    g_pSwapChain->Present(1, 0); // Present with vsync
//g_pSwapChain->Present(0, 0); // Present without vsync

    return window.open;

}