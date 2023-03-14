cmake_minimum_required(VERSION 3.16)

project(OccDebugger VERSION 0.1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

enable_language(C)

#qtpropertybrowser does not have good cmake support.
#address below may need to be changed when installing qtpropertybrowser in a different way
#other than apt-get install -y libqtpropertybrowser-dev

find_package(Qt5 REQUIRED COMPONENTS Widgets Core OpenGL Gui Xml)
find_package(Boost REQUIRED)
find_package(OpenCASCADE REQUIRED)

# the -I flag in gcc
include_directories(
  ${OpenCASCADE_INCLUDE_DIR}
)

link_directories(
    ${OpenCASCADE_LIBRARY_DIR}
    )

ADD_LIBRARY(COMMON_CONFIG STATIC ${COMMON_CONFIG_FILES})
target_link_libraries(COMMON_CONFIG
    Qt5::Widgets
    Qt5::Core
    Qt5::OpenGL
    Qt5::Gui
    Qt5::Xml
    ${OpenCASCADE_LIBRARIES}
)

#OccDebugger
file(GLOB OccDebugger_CONFIG_FILES
     "${PROJECT_SOURCE_DIR}/OccDebugger/*.h"
     "${PROJECT_SOURCE_DIR}/OccDebugger/*.cpp"
)
foreach(src_file ${OccDebugger_CONFIG_FILES})
  set_source_files_properties(
    ${src_file}
    PROPERTIES
    COMPILE_FLAGS " -g"
  )
endforeach(src_file ${code-gen})

add_executable(OccDebugger ${OccDebugger_CONFIG_FILES})
target_link_libraries(OccDebugger PRIVATE
    )