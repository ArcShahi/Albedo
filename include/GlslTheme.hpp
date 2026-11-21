#pragma once

// LLM Generated

#include <TextEditor.h>
#include <languages/TextEditorGlsl.h>

#include <iterator>

namespace Rem {

#pragma region GITHUB_PALLETE
// "0xRRGGBB" -> ImU32 (constexpr functions are implicitly inline, so this is
// header-safe)
constexpr ImU32 Hex(unsigned rgb, unsigned alpha = 255) {
  return IM_COL32((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF, alpha);
}

inline TextEditor::Palette GithubDarkPalette() {
  using C = TextEditor::Color;
  TextEditor::Palette p{
      TextEditor::GetDarkPalette()}; // bracket colours stay default
  auto set = [&](C c, ImU32 v) { p[static_cast<size_t>(c)] = v; };

  set(C::background, Hex(0x0d1117));
  set(C::text, Hex(0xe6edf3));
  set(C::identifier, Hex(0xd2a8ff));
  set(C::punctuation, Hex(0xe6edf3));
  set(C::keyword,      Hex(0xff7b72));      // float, vec4, uniform, return...
  set(C::preprocessor, Hex(0xff7b72)); // #version
  set(C::number,       Hex(0x79c0ff));
  set(C::string,       Hex(0xa5d6ff));
  set(C::comment,      Hex(0x8b949e));
  set(C::declaration,  Hex(0xffa657)); // uniforms, gl_* variables
  set(C::cursor,       Hex(0x58a6ff));
  set(C::selection,    Hex(0x388bfd, 102)); // translucent blue
  set(C::lineNumber,   Hex(0x6e7681));
  set(C::currentLineNumber, Hex(0xe6edf3));
  return p;
}

#pragma endregion

#pragma region GLSL_COLORS

inline const TextEditor::Language *GlslLang() {

  static const TextEditor::Language language{[] {
    TextEditor::Language l{*TextEditorGlsl::getDefinition()};

    constexpr const char *GlslFunctions[]{"main",          "radians",
                                          "degrees",       "sin",
                                          "cos",           "tan",
                                          "asin",          "acos",
                                          "atan",          "sinh",
                                          "cosh",          "tanh",
                                          "pow",           "exp",
                                          "log",           "exp2",
                                          "log2",          "sqrt",
                                          "inversesqrt",   "abs",
                                          "sign",          "floor",
                                          "trunc",         "round",
                                          "ceil",          "fract",
                                          "mod",           "modf",
                                          "min",           "max",
                                          "clamp",         "mix",
                                          "step",          "smoothstep",
                                          "fma",           "length",
                                          "distance",      "dot",
                                          "cross",         "normalize",
                                          "faceforward",   "reflect",
                                          "refract",       "transpose",
                                          "determinant",   "inverse",
                                          "lessThan",      "lessThanEqual",
                                          "greaterThan",   "greaterThanEqual",
                                          "equal",         "notEqual",
                                          "any",           "all",
                                          "not",           "texture",
                                          "textureLod",    "textureSize",
                                          "texelFetch",    "textureGrad",
                                          "textureOffset", "dFdx",
                                          "dFdy",          "fwidth"};

    constexpr const char *GlslVariables[]{
        "uTime",         "uResolution",   "uMouse",
        "uTexture",      "vUv",           "FragColor",
        "gl_FragCoord",  "gl_FragDepth",  "gl_FrontFacing",
        "gl_PointCoord", "gl_PrimitiveID"};

    l.identifiers.insert(std::begin(GlslFunctions), std::end(GlslFunctions));
    l.declarations.insert(std::begin(GlslVariables), std::end(GlslVariables));
    return l;
  }()};

  return &language;
}
#pragma endregion
} // namespace Rem