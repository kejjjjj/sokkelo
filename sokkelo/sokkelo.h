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

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];

#define DotProduct(x,y)			((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define VectorSubtract(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(a,b,c)		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define	VectorScale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	VectorMA(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))


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
#include <array>

#include "resource.h"
#include "framework.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_stdlib.h"
#include "ImGui/imgui_internal.h"

#include <d3d11.h>
#include <d3dx11.h>

#include "qmath.hpp"

#include "d_common_shapes.hpp"

#include "filetools.hpp"
#include "winfont.hpp"
#include "imageresources.hpp"

#include "appwindows.hpp"
#include "basewindow.hpp"
#include "basetabs.hpp"

#include "app_ui.hpp"
#include "maze_gen.hpp"

#include "timing.hpp"

#include "whattheheck.hpp"

using namespace std::chrono_literals;


#endif