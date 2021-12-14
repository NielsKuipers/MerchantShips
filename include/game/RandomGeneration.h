//
// Created by Niels on 09/12/21.
//

#ifndef MERCHANTSHIPS_RANDOMGENERATION_H
#define MERCHANTSHIPS_RANDOMGENERATION_H

#include <random>

namespace RNG
{
    static std::random_device rd;
    inline static std::mt19937 randEngine{std::mt19937(rd())};

    static int generateRandomNumber(const int end)
    {
        std::uniform_int_distribution<int> dist(0, end);
        return dist(randEngine);
    }

    static int generateRandomNumber(const int min, const int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(randEngine);
    }
}

#endif //MERCHANTSHIPS_RANDOMGENERATION_H
