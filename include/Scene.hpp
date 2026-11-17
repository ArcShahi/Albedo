
#pragma once

#include <Rem/App.hpp>
#include <Rem/Renderer/Common.hpp>
#include <Rem/Renderer/Shader.hpp>

#include <optional>

class SceneLayer : public Rem::Layer {
public:
  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;

private:
  Rem::VertexArray vao_{};
  std::optional<Rem::VertexBuffer> vbo_{}; // Created in OnAttact 
  Rem::IndexBuffer ibo_{};
  Rem::Shader shader_{};
  Rem::Texture texture_{ASSETS "/albedo.png"};
};