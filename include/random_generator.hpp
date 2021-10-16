#pragma once

#include <random>

class RandomGenerator
{
private:
    std::mt19937 m_gen{ std::random_device{}() };

public:
    RandomGenerator() = default;
    virtual ~RandomGenerator(void) = default;

    bool randomChance(float chance);
    float randomFloat(float min, float max);
};
