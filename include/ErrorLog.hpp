#pragma once

#include <cstdio>
#include <format>
#include <print>
#include <string_view>
#include <utility>

namespace Rem::Log {

namespace Hood {
template <typename... Ts>
void Write(std::FILE *out, std::string_view color, std::string_view tag,
           std::format_string<Ts...> fmt, Ts &&...args) noexcept {

  std::println(out, "{} [ {} ] : {}\033[0m", color, tag,
               std::format(fmt, std::forward<Ts>(args)...));
}
} // namespace Hood


template <typename... Ts>
void Error(std::format_string<Ts...> fmt, Ts &&...args) {
  Hood::Write(stderr, "\033[91m", "ERROR", fmt, std::forward<Ts>(args)...);
}

template <typename... Ts>
void Info(std::format_string<Ts...> fmt, Ts &&...args) {
  Hood::Write(stdout, "\033[92m", "INFO", fmt, std::forward<Ts>(args)...);
}


} // namespace Rem::Log