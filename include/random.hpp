#ifndef RANDOM_HPP
#define RANDOM_HPP

[[nodiscard]]
auto randomDouble() noexcept -> double;

[[nodiscard]]
auto randomDouble(double min, double max) noexcept -> double;

#endif
