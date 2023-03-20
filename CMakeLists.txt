cmake_minimum_required(VERSION 3.16)

project(occdebuggger VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

enable_language(C)

#qtpropertybrowser does not have good cmake support.
#address below may need to be changed when installing qtpropertybrowser in a different way
#other than apt-get install -y libqtpropertybrowser-dev

find_package(Qt5 REQUIRED COMPONENTS Widgets Core OpenGL Gui Xml Network)
find_package(Boost REQUIRED)
find_package(OpenCASCADE REQUIRED)

# the -I flag in gcc
include_directories(
  ${OpenCASCADE_INCLUDE_DIR}
  ${Qt5_INCLUDE_DIR}
)

link_directories(
    ${OpenCASCADE_LIBRARY_DIR}
    ${Qt5_LIBRARY_DIR}
    )

#shared src
file(GLOB COMMON_CONFIG_FILES
    "${PROJECT_SOURCE_DIR}/src/ViewData/*.h"
    "${PROJECT_SOURCE_DIR}/src/ViewData/*.cpp"
    )

foreach(src_file ${COMMON_CONFIG_FILES})
  set_source_files_properties(
    ${src_file}
    PROPERTIES
    COMPILE_FLAGS "${COMPILE_FLAGS} ${flags_1} -g"
  )
endforeach(src_file ${front-end})
ADD_LIBRARY(COMMON_CONFIG STATIC ${COMMON_CONFIG_FILES})
target_link_libraries(COMMON_CONFIG
    Qt5::Widgets
    Qt5::Core
    Qt5::OpenGL
    Qt5::Gui
    Qt5::Xml
    Qt5::Network
    ${OpenCASCADE_LIBRARIES}
)

#OccConfig
file(GLOB OCC_CONFIG_FILES
     "${PROJECT_SOURCE_DIR}/src/*.h"
     "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
foreach(src_file ${OCC_CONFIG_FILES})
  set_source_files_properties(
    ${src_file}
    PROPERTIES
    COMPILE_FLAGS " -g"
  )
endforeach(src_file ${code-gen})

add_executable(OccConfig ${OCC_CONFIG_FILES})
target_link_libraries(OccConfig PRIVATE
    COMMON_CONFIG
    )
