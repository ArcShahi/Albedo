#include <Scene.hpp>
#include <array>

#include <glad/glad.h> // for glGetTextureLevelParameteriv
#include <imgui.h>

namespace {

// Shows a preview scaled to the panel width plus basic GL info about the
// texture
void DrawTextureInfo(const char *label, GLuint handle, int w, int h,
                     bool flipV) {
  ImGui::PushID(label);

  GLint internalFormat{};
  glGetTextureLevelParameteriv(handle, 0, GL_TEXTURE_INTERNAL_FORMAT,
                               &internalFormat);

  ImGui::SeparatorText(label);
  ImGui::Text("Handle : %u", handle);
  ImGui::Text("Size   : %d x %d", w, h);
  ImGui::Text("Format : 0x%X", internalFormat); // e.g. 0x8058 = GL_RGBA8

  const float avail = ImGui::GetContentRegionAvail().x;
  const float aspect = h > 0 ? static_cast<float>(w) / h : 1.0f;
  const ImVec2 size{avail, avail / aspect};

  // FBO color attachments are bottom-up in GL, so flip V for those
  const ImVec2 uv0 = flipV ? ImVec2(0, 1) : ImVec2(0, 0);
  const ImVec2 uv1 = flipV ? ImVec2(1, 0) : ImVec2(1, 1);
  ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(handle)), size,
               uv0, uv1);

  ImGui::PopID();
}

} // namespace

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

  //

  if (viewportSize_.x > 0 && viewportSize_.y > 0 &&
      (viewportSize_.x != target_.width || viewportSize_.y != target_.height)) {
    // recreate
    Rem::DestroyFrameBuffer(fbo_);
    Rem::DestroyTexture(target_);
    target_ = Rem::CreateTexture(static_cast<int>(viewportSize_.x),
                                 static_cast<int>(viewportSize_.y));
    fbo_ = Rem::CreateFrameBufferWithTexture(target_);
  }

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

void SceneLayer::OnUIRender() {

  // Scene
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Scene");
  const ImVec2 avail{ImGui::GetContentRegionAvail()};
  viewportSize_ = glm::ivec2(avail.x, avail.y);
  ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(target_.handle)),
               avail, ImVec2(0, 1), ImVec2(1, 0));
  ImGui::End();
  ImGui::PopStyleVar();
}
