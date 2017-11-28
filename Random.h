#pragma once

#include <math/vec3.hpp>

namespace Random {
int GetInt(int min, int max);
float GetFloat(float min, float max);
float GetNormalizedFloat();
float GetMinus1to1Float();
math::vec3 GetNormalizedVec3();
}
