cmake_minimum_required(VERSION 3.16)

project(
  occdebuggger
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

enable_language(C)


find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL Gui)
find_package(Boost REQUIRED )


include_directories(${OpenCASCADE_INCLUDE_DIR} ${Qt5_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/build/OCC_CONFIG_autogen/include)

link_directories(${OpenCASCADE_LIBRARY_DIR} ${Qt5_LIBRARY_DIR})

# OccConfig
file(GLOB OCC_CONFIG_FILES
    #"${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
     )

foreach(src_file ${OCC_CONFIG_FILES})
  set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS " -g")
endforeach(src_file ${code-gen})

add_executable(OccConfig ${OCC_CONFIG_FILES}
                         ${PROJECT_SOURCE_DIR}/src/OccDebuger.rc)

target_link_libraries(OccConfig PRIVATE Qt5::Core Qt5::Widgets Qt5::OpenGL
                                        Qt5::Gui     ${Boost_LIBRARIES})
