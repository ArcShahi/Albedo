#include <App.hpp>

int main() { 

  Albedo albedo{};
  if (!albedo.Init()) {
    return -1;
  }
  albedo.Run();
}