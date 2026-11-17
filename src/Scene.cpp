#include <Scene.hpp>
#include <array>

void SceneLayer::OnAttach() {

  // Map Postion and Texture coord : Texture coord (0,0) from bottom left
  // instead of at center
/*  std::array<float, 20> vertex_data{
      // pos                 uv
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // 0 top right
      -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, // 1 top left
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // 2 bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f  // 3 bottom left
  };*/
  std::array<float, 16> vertex_data{
      // pos                 uv
      1.0f,  1.0f, 1.0f, 1.0f, // 0 top right
      -1.0f, 1.0f, 0.0f, 1.0f, // 1 top left
      1.0f,  -1.0, 1.0f, 0.0f, // 2 bottom right
      -1.0f, -1.0, 0.0f, 0.0f  // 3 bottom left
  };

  std::array<uint32_t, 6> indices{0, 1, 3, 0, 3, 2};

  Rem::VertexBuffer tmpVB{
      vertex_data.data(),
      static_cast<uint32_t>(vertex_data.size() * sizeof(float))};

  ibo_.Init(indices.data(), static_cast<uint32_t>(indices.size()));

  // Read from the specified buffer
  vao_.VertexBufferInfo(0, tmpVB.GetHandle(), 4 * sizeof(float), 0);
  vao_.IndextBufferInfo(ibo_.GetHandle());

  // Link Attrib 0 that has 2 elements of type float and offset 0 in vertex
  // data to slot 0 of VA
  vao_.AddAttrib<float, 2>(0, 0);

  // Link attrib 1 that has 2 elements of type float and offset of 12 B
  vao_.AddAttrib<float, 2>(1, 2 * sizeof(float));

  // Bind the texture
  texture_.bind(0);

  // Shader
  std::string vsPath{SHADERS + std::string("/vertex.shader")};
  std::string fsPath{SHADERS + std::string("/fragment.shader")};

  shader_.Init(vsPath, fsPath);
  shader_.Use();
  shader_.SetUniform("uTexture", 0); // out texture is bound to slot 0
}

void SceneLayer::OnDetach() { std::cout << "Scene yanked\n"; }

void SceneLayer::OnUpdate([[maybe_unused]]float dt) {

  // For CPU sdie logic will fuck rendering
}

void SceneLayer::OnRender() {

  // Clear Screen
  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Update Shader
  shader_.Use();
  shader_.SetUniform("uTime", Rem::App::GetTime());

  // Draw
  vao_.Bind();
  glDrawElements(GL_TRIANGLES, ibo_.ElementCount(), GL_UNSIGNED_INT, nullptr);
}
