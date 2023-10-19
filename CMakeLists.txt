cmake_minimum_required(VERSION 3.16)

project(
  occdebuggger
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

enable_language(C)

# OccConfig
file(GLOB OCC_CONFIG_FILES
    "${PROJECT_SOURCE_DIR}/src/math_utils.h"
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
     )

add_executable(OccConfig ${OCC_CONFIG_FILES}
                        )
