#pragma once

#include <math/vec3.hpp>

class Random_Impl;

class Random {
    std::unique_ptr<Random_Impl> impl_;
public:
    explicit Random(uint32_t seed);
    ~Random() = default;

    int GetInt(int min, int max);
    float GetFloat(float min, float max);
    float GetNormalizedFloat();
    float GetMinus1to1Float();
    math::vec3 GetNormalizedVec3();

    double GetNormalizedDouble();
};
