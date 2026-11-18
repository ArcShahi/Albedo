#include <Scene.hpp>
#include <array>

void SceneLayer::OnAttach() {

  // Will be smapled by shader
  texture_ = Rem::LoadTexture(ASSETS "/albedo.png");
  if (!texture_.handle) {
    std::cerr << "Couldn't load texture\n";
  }

  // Render target : need to resize later
  target_ = Rem::CreateTexture(1280, 720);
  fbo_ = Rem::CreateFrameBufferWithTexture(target_);
  if (!fbo_.handle) {
    std::cerr << "Framebuffer fucked\n";
  }

  shader_.Init(SHADERS + std::string("/vertex.shader"),
               SHADERS + std::string("/fragment.shader"));

  shader_.Use();
  shader_.SetUniform("uTexture", 0); // sampler will fetch fromn this
}

void SceneLayer::OnDetach() {
  Rem::DestroyFrameBuffer(fbo_);
  Rem::DestroyTexture(target_);
  Rem::DestroyTexture(texture_);
}

void SceneLayer::OnUpdate([[maybe_unused]] float dt) {

  // For CPU sdie logic will fuck rendering
}

void SceneLayer::OnRender() {

  // TODO IMGUI stuff

  int window[4]{};
  glGetIntegerv(GL_VIEWPORT, window);

  // Draw to texture
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_.handle);
  glViewport(0, 0, target_.width, target_.height);

  // Clear Screen
  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Update Shader
  shader_.Use();
  shader_.SetUniform("uTime", Rem::App::GetTime());

  // Draw
  glBindTextureUnit(0, texture_.handle);
  vao_.Bind();
  glDrawArrays(GL_TRIANGLES, 0, 3);

  // Back to the window
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(window[0], window[1], window[2], window[3]);
}
