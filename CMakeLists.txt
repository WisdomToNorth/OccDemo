cmake_minimum_required(VERSION 3.16)

if(WIN32)
    set(USER_SET_FILE ${CMAKE_CURRENT_SOURCE_DIR}/WinUserSetting.cmake)
    if(NOT EXISTS ${USER_SET_FILE})
        file(WRITE ${USER_SET_FILE} "# set(CMAKE_TOOLCHAIN_FILE D:/CODE/vcpkg/scripts/buildsystems/vcpkg.cmake)")
    endif()
    include(${USER_SET_FILE})
endif()

project(
  occdebuggger
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

enable_language(C)

find_package(GTest REQUIRED)

enable_testing()
include(GoogleTest)


if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
    message(STATUS "Using Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++20")
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

# OccConfig
set(OCC_CONFIG_FILES
    src/public_function.h
    src/public_function.cpp
    src/testdata.h
    src/lc_struct.h
    threadpool.h
    threadpool.cpp
    main.cpp
    )


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)

add_executable(OccConfig ${OCC_CONFIG_FILES})

target_include_directories(OccConfig
PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(OccConfig
PRIVATE
    ${Qt_LIBRARIES}
    ${GTEST_LIBRARIES}
)

gtest_add_tests(TARGET OccConfig
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/executable)
