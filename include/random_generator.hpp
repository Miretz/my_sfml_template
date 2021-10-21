#pragma once

#include <random>

class RandomGenerator
{
public:
    bool randomChance(float chance);
    float randomFloat(float min, float max);

private:
    std::mt19937 gen_{ std::random_device{}() };
};
