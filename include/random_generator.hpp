#pragma once

#include <random>

class RandomGenerator
{
public:
    auto randomChance(float chance) -> bool;
    auto randomFloat(float min, float max) -> float;

private:
    std::mt19937 gen_{ std::random_device{}() };
};
