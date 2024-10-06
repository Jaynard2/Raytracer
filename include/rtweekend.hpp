#pragma  once


#include <random>
constexpr double PI = 3.1415926535897932385;

constexpr inline double degreeToRadians(double degrees)
{
    return degrees * PI / 180.0;
}

inline double randomDouble()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 gen;
    return distribution(gen);
}

inline double randomDouble(double min, double max)
{
    return min + (max - min) * randomDouble();
}

inline int randomInt(int min, int max)
{
    return static_cast<int>(randomDouble(min, max + 1));
}
