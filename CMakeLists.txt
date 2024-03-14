cmake_minimum_required(VERSION 3.16)

project(
  occdebuggger
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

enable_language(C)

# if(WIN32)
#     find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL Gui)
#     set(Qt_LIBRARIES Qt5::Core Qt5::Widgets Qt5::Gui)
#     set(Qt_LIBRARY_DIR ${Qt5_LIBRARY_DIR})

# elseif(APPLE)
#     find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL Gui)
#     set(Qt_LIBRARIES Qt6::Core Qt6::Widgets Qt6::Gui)
#     set(Qt_LIBRARY_DIR ${Qt6_LIBRARY_DIR})
#     set(Qt_INCLUDE_DIR ${Qt6_INCLUDE_DIRS})

# elseif(UNIX)
#     find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL Gui)
#     set(Qt_LIBRARIES Qt5::Core Qt5::Widgets Qt5::Gui)
#     set(Qt_LIBRARY_DIR ${Qt5_LIBRARY_DIR})
# endif()


# QT
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Gui REQUIRED)

set(Qt_LIBRARIES
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui)

set(Qt_LIBRARY_DIR ${Qt${QT_VERSION_MAJOR}_LIBRARY_DIR})
set(Qt_INCLUDE_DIR ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS})

find_package(GTest REQUIRED)
find_package(FMT CONFIG REQUIRED)
#find_package(OpenCASCADE CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

enable_testing()
include(GoogleTest)


if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
    message(STATUS "Using Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++20")
endif()

include_directories(
    ${Qt_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/src
)

# OccConfig
set(OCC_CONFIG_FILES
    src/public_function.h
    src/public_function.cpp
    src/testdata.h
    src/lc_struct.h
    main.cpp
    )

add_executable(OccConfig ${OCC_CONFIG_FILES})

target_include_directories(OccConfig
PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(OccConfig
PRIVATE
    ${Qt_LIBRARIES}
)