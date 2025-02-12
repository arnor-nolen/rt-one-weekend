#include <halt.hpp>

#include <exception>
#include <print>

void halt(std::string_view message) {
    std::println("{}", message);
    std::terminate();
}
