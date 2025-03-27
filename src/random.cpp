#include <random.hpp>

#include <random>

auto randomDouble() noexcept -> double
{
    static auto s_distribution
        = std::uniform_real_distribution<double>{ 0.0, 1.0 };
    // NOLINTNEXTLINE(cert-msc32-c, cert-msc51-cpp)
    thread_local static auto s_generator = std::mt19937{ 3 };

    return s_distribution(s_generator);
};

auto randomDouble(double min, double max) noexcept -> double
{
    return min + (max - min) * randomDouble();
};

auto randomInt(int min, int max) noexcept -> int
{
    return static_cast<int>(randomDouble(min, max + 1));
}
