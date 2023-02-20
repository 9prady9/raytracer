include(FetchContent)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8
)

if (NOT glm_POPULATED)
  FetchContent_Populate(glm)
endif ()


file(DOWNLOAD https://github.com/CLIUtils/CLI11/releases/download/v2.1.2/CLI11.hpp
  ${PROJECT_BINARY_DIR}/third_party/include/CLI11.hpp
  EXPECTED_MD5 9f2f50f203b29c00b50a4c8ef2111346
)


FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb
  GIT_TAG af1a5bc352164740c1cc1354942b1c6b72eacb8a # Sep 10, 2021
)

if (NOT stb_POPULATED)
  FetchContent_Populate(stb)
endif ()

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.10.5
)

if (NOT json_POPULATED)
  FetchContent_Populate(json)
endif ()
