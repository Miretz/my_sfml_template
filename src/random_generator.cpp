#include "random_generator.hpp"

auto RandomGenerator::randomFloat(float min, float max) -> float
{
    float lessThanZero = 0.f;
    if (min < 0.f)
    {
        if (max < 0.f)
        {
            std::uniform_real_distribution<float> rFloat{ std::abs(min), std::abs(max) };
            return rFloat(gen_) * -1;
        }
        else
        {
            max += std::abs(min);
            lessThanZero = std::abs(min);
            min = 0;
        }
    }
    std::uniform_real_distribution<float> rFloat{ min, max };
    return rFloat(gen_) - lessThanZero;
}

auto RandomGenerator::randomChance(float chance) -> bool
{
    std::bernoulli_distribution rChance{ chance };
    return rChance(gen_);
}