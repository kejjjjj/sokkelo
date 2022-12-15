#pragma once

#ifndef qmath
#define qmath

#ifndef M_PI
#define M_PI        3.14159265358979323846f // matches value in gcc v2 math.h
#endif

#include "sokkelo.h"

#ifndef M_PI
#define M_PI        3.14159265358979323846f // matches value in gcc v2 math.h
#endif

void AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
void vectoangles(const vec3_t value1, vec3_t angles);
ImVec2 VectorsToAngles(const ImVec2& src, const ImVec2& dst);


#endif