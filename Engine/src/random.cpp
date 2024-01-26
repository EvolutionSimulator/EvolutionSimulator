#include "Random.h"

std::random_device Random::rd;
std::mt19937_64 Random::engine(rd());
