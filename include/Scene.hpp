
#pragma once

#include <Rem/App.hpp>
#include <Rem/Renderer/Renderer.hpp>
#include <Rem/Renderer/Shader.hpp>
#include <Rem/Renderer/VertexArray.hpp>


class SceneLayer : public Rem::Layer {
public:
  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;

private:
  Rem::Shader shader_{};
  Rem::FrameBuffer fbo_{};
  Rem::Texture texture_{};
  Rem::Texture target_{};
  Rem::VertexArray vao_{};
};