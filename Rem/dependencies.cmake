# LLM Generated 

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)   

# ---------- GLFW 3.5.1 ----------
if(NOT TARGET glfw)
  set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

  FetchContent_Declare(glfw
    URL      https://github.com/glfw/glfw/releases/download/3.5.1/glfw-3.5.1.zip
    URL_HASH SHA256=ea79bc5feffc254c87291980c2d0bce9acebb68c4983b79f961dcd2cb8a611a0
    DOWNLOAD_EXTRACT_TIMESTAMP OFF)
  FetchContent_MakeAvailable(glfw)
endif()

# ---------- GLM 1.0.3 ----------
if(NOT TARGET glm::glm)
  set(GLM_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
  set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)

  FetchContent_Declare(glm
    URL      https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip
    URL_HASH SHA256=1c0a0fced9b0d87c7b7bc94e40be490cff6d4c83c25db8488d8f33754e7fdeb2
    DOWNLOAD_EXTRACT_TIMESTAMP OFF)
  FetchContent_MakeAvailable(glm)
endif()