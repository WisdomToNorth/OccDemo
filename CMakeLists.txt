cmake_minimum_required(VERSION 3.16)

project(
  occdebuggger
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

enable_language(C)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL Gui)
set(Qt_LIBRARIES Qt5::Core Qt5::Widgets Qt5::Gui)
set(Qt_LIBRARY_DIR ${Qt5_LIBRARY_DIR})

# OccConfig
file(GLOB OCC_CONFIG_FILES
    "${PROJECT_SOURCE_DIR}/datautils.h"
    "${PROJECT_SOURCE_DIR}/datautils.cpp"
    "${PROJECT_SOURCE_DIR}/main.cpp"
     )



add_executable(OccConfig ${OCC_CONFIG_FILES}
                        )

target_include_directories(OccConfig
PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(OccConfig
PRIVATE
    ${Qt_LIBRARIES}
)