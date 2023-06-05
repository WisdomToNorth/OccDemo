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

if(WIN32)
  if(MSVC)
    option(USE_MP "use multiple" ON)
    option(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP
           "Set The Global Option COMPILE_FLAGS /MP to target." ON)
    if(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP OR USE_MP)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    endif()
    set(VS_STARTUP_PROJECT ${PROJECT_NAME})
  endif(MSVC)
endif()

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
  set(CMAKE_PREFIX_PATH "D:/OpenSource/Qt/5.15.2/msvc2019_64")
  # list(APPEND CMAKE_PREFIX_PATH "指定版本库的安装路径")
endif()

find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL Gui)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
  message("current platform: Windows ")
  # Set OpenCASCADE paths to headers, binaries, libraries, resources, tests,
  # samples, data
  set(OpenCASCADE_INSTALL_PREFIX D:/OpenSource/opencascade-7.7.0)
  set(OpenCASCADE_LIBRARY_DIR "${OpenCASCADE_INSTALL_PREFIX}/lib")
  set(OpenCASCADE_INCLUDE_DIR "${OpenCASCADE_INSTALL_PREFIX}/inc")

  # List of available OpenCASCADE libraries.
  set(OpenCASCADE_LIBRARIES
      TKernel;TKMath;TKG2d;TKG3d;TKGeomBase;TKBRep;TKGeomAlgo;TKTopAlgo;TKPrim
      TKBO;TKShHealing;TKBool;TKHLR;TKFillet;TKOffset;TKFeat;TKMesh;TKXMesh;TKService;TKV3d;TKOpenGl
      TKMeshVS;TKCDF;TKLCAF;TKCAF;TKBinL;TKXmlL;TKBin;TKXml;TKStdL;TKStd;TKTObj;TKBinTObj;TKXmlTObj
      TKVCAF;TKXDE;TKXSBase;TKSTEPBase;TKSTEPAttr;TKSTEP209;TKSTEP;TKIGES;TKXCAF;TKXDEIGES;TKXDESTEP
      TKSTL;TKVRML;TKRWMesh;TKXmlXCAF;TKBinXCAF;TKXDECascade;TKExpress;TKDraw;TKTopTest
      TKOpenGlTest;TKViewerTest;TKXSDRAW;TKDCAF;TKXDEDRAW;TKTObjDRAW;TKQADraw)

elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
  message("current platform: Linux ")
  find_package(OpenCASCADE REQUIRED)

else()
  message("current platform: unkonw ")
endif()

# the -I flag in gcc
include_directories(${OpenCASCADE_INCLUDE_DIR} ${Qt5_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/build/OCC_CONFIG_autogen/include)

link_directories(${OpenCASCADE_LIBRARY_DIR} ${Qt5_LIBRARY_DIR})

# OccConfig
file(GLOB OCC_CONFIG_FILES
"${PROJECT_SOURCE_DIR}/src/*.h"
     "${PROJECT_SOURCE_DIR}/src/*.cpp"
     )

foreach(src_file ${OCC_CONFIG_FILES})
  set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS " -g")
endforeach(src_file ${code-gen})

add_executable(OccConfig ${OCC_CONFIG_FILES}
                         ${PROJECT_SOURCE_DIR}/src/OccDebuger.rc)

target_link_libraries(OccConfig PRIVATE Qt5::Core Qt5::Widgets Qt5::OpenGL
                                        Qt5::Gui ${OpenCASCADE_LIBRARIES})

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/build/Files)
add_custom_command(
  TARGET OccConfig
  POST_BUILD
  COMMAND cp ${PROJECT_SOURCE_DIR}/Files/*.qss ${PROJECT_SOURCE_DIR}/build/Files
  COMMAND cp ${PROJECT_SOURCE_DIR}/Files/uiconfig.json
          ${PROJECT_SOURCE_DIR}/build/Files)
