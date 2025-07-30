cmake_minimum_required(VERSION 3.10)
project(Renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(lib_dir "${PROJECT_SOURCE_DIR}/lib/")

file(GLOB source_files "src/*.cpp")
file(GLOB u8g2_core_sources "${lib_dir}/u8g2/csrc/*.c")
file(GLOB u8g2_sdl_sources "${lib_dir}/u8g2/sys/sdl/common/*.c")
file(GLOB baduilib_sources "${lib_dir}/baduilib/*.cpp")

add_library(u8g2 STATIC ${u8g2_core_sources} ${u8g2_sdl_sources})
add_library(baduilib STATIC ${baduilib_sources})

find_package(SDL2 REQUIRED)

target_include_directories(u8g2 PUBLIC "${lib_dir}/u8g2/csrc/" ${SDL2_INCLUDE_DIRS})
target_include_directories(baduilib PUBLIC "${lib_dir}/baduilib/" "${lib_dir}/u8g2/csrc/")
target_link_libraries(u8g2 PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(baduilib PUBLIC u8g2)

add_executable(Renderer ${source_files})
target_link_libraries(Renderer PRIVATE u8g2 baduilib)
