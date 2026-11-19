#include <App.hpp>

int main() {

  Rem::Albedo albedo{};
  if (!albedo.Init()) {
    return -1;
  }
  albedo.Run();
}