#ifndef HALT_HPP
#define HALT_HPP

#include <string_view>

[[noreturn]]
void halt(std::string_view message);

#endif
