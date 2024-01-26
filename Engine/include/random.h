#ifndef RANDOM_H
#define RANDOM_H

#pragma once

#include <random>
#include <cstdint>

class Random
{
private:
    static std::random_device rd; //Tbh I think this is a lil useless but I can't think of a better way rn
    static std::mt19937_64 engine;

public:
    template<typename U, typename V>
    static int Int(U min, V max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(engine);
    }

    template<typename U, typename V>
    static double Double(U min, V max)
    {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(engine);
    }

    template<typename U, typename V>
    static double Normal(U mean, V sigma)
    {
        std::normal_distribution<double> dist(mean, sigma);
        return dist(engine);
    }

    template<typename T>
    static T& ChooseRandom(T& first, T& second)
    {
        std::uniform_int_distribution<int> dist(0, 1);

        if (dist(engine) == 0)
            return first;
        return second;
    }

    static void SetSeed(uint64_t seed)
        {
            engine.seed(seed);
        }
};
#endif // RANDOM_H
