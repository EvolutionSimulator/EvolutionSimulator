#include "core/random.h"

std::random_device Random::rd;
thread_local std::mt19937_64 Random::engine;
