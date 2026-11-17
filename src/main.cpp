#include <Rem/App.hpp>
#include <iostream>
#include <cmath>


class SceneLayer : public Rem::Layer {
public:
  void OnAttach() override { std::cout << "Scene layer added\n";
  }
  void OnDetach() override { std::cout << "Scene yanked\n"; }

  void OnUpdate([[maybe_unused]] float dt) override {

       std::cout << '.';
  }

  void OnUIRender() override {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  }
};

int main() {

  Rem::AppSpecs specs{
      .name = "Albedo",
      .windowSpecs = {.title = "Albedo", .width = 1280, .height = 720}};

  Rem::App Albedo{specs};
  Albedo.PushLayer<SceneLayer>();
  Albedo.Run();

}