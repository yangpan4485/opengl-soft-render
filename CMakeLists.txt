cmake_minimum_required(VERSION 3.0)
project(soft_render)

# 设置C++的版本号
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/EGL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/GLES2)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/GLES2/context)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/render_pipeline)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/render_pipeline/compiler)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/example/example_common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/example/shader_compiler)
add_definitions(-DFRAME_CALLBACK)
# -Wall 
add_definitions("-g")

file(GLOB_RECURSE CPP_SOURCE "src/*.cc" "src/*.cpp" "src/*.c" "src/*.h" "src/*.hpp")
file(GLOB_RECURSE EXAMPLE_SOURCE "example/*.cc" "example/*.cpp" "example/*.c" "example/*.h" "example/*.hpp")

set(RENDER_SOURCE ${CPP_SOURCE} ${EXAMPLE_SOURCE})
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${RENDER_SOURCE})
add_executable(render_demo ${RENDER_SOURCE})
target_link_libraries(render_demo X11 pthread)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT render_demo)
