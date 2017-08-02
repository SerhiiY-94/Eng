#include "Random.h"

#include <random>
#include <mutex>

static std::mt19937 rd((unsigned int) time(0));
static std::default_random_engine gen(rd());
static std::mutex mtx;
std::uniform_real_distribution<float> n_float_distr(0.0f, 1.0f);
std::uniform_real_distribution<float> minus_1_to_1_float_distr(-1.0f, 1.0f);

int Random::GetInt(int min, int max) {
    std::lock_guard<std::mutex> lck(mtx);
    std::uniform_int_distribution<int> distr(min, max);
    return distr(gen);
}

float Random::GetFloat(float min, float max) {
    std::lock_guard<std::mutex> lck(mtx);
    std::uniform_real_distribution<float> distr(min, max);
    return distr(gen);
}

float Random::GetNormalizedFloat() {
    return n_float_distr(gen);
}

float Random::GetMinus1to1Float() {
    return minus_1_to_1_float_distr(gen);
}

math::vec3 Random::GetNormalizedVec3() {
    return math::normalize_fast(
            math::vec3{ minus_1_to_1_float_distr(gen),
                        minus_1_to_1_float_distr(gen),
                        minus_1_to_1_float_distr(gen) });
}