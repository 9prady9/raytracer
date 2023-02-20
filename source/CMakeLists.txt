include(tonkytin_dependencies)

# TODO(pradeep) Use cmake file sets once it is available in 3.23
target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp

    fractal.cpp
    fractal.hpp
    material.cpp
    material.hpp
    perlin_noise.cpp
    perlin_noise.hpp
    math_utils.hpp
)

target_include_directories(${PROJECT_NAME}
  SYSTEM PRIVATE
    ${glm_SOURCE_DIR}
  SYSTEM PRIVATE
    ${stb_SOURCE_DIR}
  SYSTEM PRIVATE
    ${PROJECT_BINARY_DIR}/third_party/include
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    STB_IMAGE_IMPLEMENTATION
    TONKYTIN_ASSETS_ROOT="${PROJECT_SOURCE_DIR}/assets"
)
