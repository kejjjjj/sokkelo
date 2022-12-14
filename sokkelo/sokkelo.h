#pragma once

#ifndef epicengine_h
#define epicengine_h

#define NOMINMAX

#pragma warning(disable : 4996)
#pragma warning(disable : 4005) //DIRECTX11 STOP!!!!

#ifdef _WIN64
typedef unsigned long long UPTR;
#else
typedef unsigned long UPTR;
#endif

#include <Windows.h>
#include <thread>
#include <vector>
#include <iostream>
#include <math.h>
#include <functional>
#include <utility>
#include <filesystem>
#include <fstream>
#include <direct.h>
#include <timeapi.h>

#include "resource.h"
#include "framework.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_stdlib.h"
#include "ImGui/imgui_internal.h"

#include <d3d11.h>
#include <d3dx11.h>

#include "d_common_shapes.hpp"

#include "filetools.hpp"
#include "winfont.hpp"
#include "imageresources.hpp"

#include "appwindows.hpp"
#include "basewindow.hpp"

#include "app_ui.hpp"

#include "timing.hpp"

#endif