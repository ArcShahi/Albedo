#pragma once

#include <iostream>
#include <string>

namespace {

constexpr auto CLEAR{"\033[0m"};
constexpr auto RED{"\033[101m"};
constexpr auto GREEN{"\033[102m"};

} // namespace

namespace Rem {

// TODO: Variadic templates
inline void LogError(const char *msg) {
  std::cerr << RED << "[ ERROR ]: " << msg << CLEAR << '\n';
}
inline void LogError(const std::string& msg) {
  std::cerr << RED << "[ ERROR ]: " << msg << CLEAR << '\n';
}

inline void LogInfo(const char *msg) {
  std::cout<< GREEN << "[ INFO ]: " << msg << CLEAR << '\n';
}
inline void LogInfo(const std::string& msg) {
  std::cout << GREEN << "[ INFO ]: " << msg << CLEAR << '\n';
}

} // namespace Rem