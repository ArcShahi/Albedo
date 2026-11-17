#include <Scene.hpp>

int main() {

  Rem::AppSpecs specs{
      .name = "Albedo",
      .windowSpecs = {.title = "Albedo", .width = 1280, .height = 720}};

  Rem::App Albedo{specs};
  Albedo.PushLayer<SceneLayer>();
  Albedo.Run();
}