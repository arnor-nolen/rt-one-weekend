#include <halt.hpp>

#include <exception>

#include <fmt/core.h>

void halt(std::string_view message) {
    fmt::print("{}\n", message);
    std::terminate();
}
