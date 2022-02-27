cmake_minimum_required(VERSION 3.22...3.23)

project(tonkytin VERSION 0.1.0
  DESCRIPTION "Hobby Raytracer Project"
  LANGUAGES CXX
)

string(TOUPPER ${PROJECT_NAME} project_uc_name)

set(popfx ${project_uc_name})   # Prefix for project options
set(ptpfx ${PROJECT_NAME})      # Prefix for project targets

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")

include(tonkytin_build_type)

set(${popfx}_COMPUTE_BACKEND "threads" CACHE STRING "Select raytracing computation backend")
set_property(CACHE ${popfx}_COMPUTE_BACKEND PROPERTY STRINGS "threads" "simd" "cuda" "opencl")

#[[
All options, find_package calls should be above this comment block.
Only code structuring, install logic should be below this block.
]]

add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    CXX_STANDARD 20
)

add_subdirectory(source)
