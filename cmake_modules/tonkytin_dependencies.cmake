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
