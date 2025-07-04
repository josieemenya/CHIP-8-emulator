cmake_minimum_required(VERSION 3.16)
project(CHIP_8)

set(CMAKE_CXX_STANDARD 20)

include_directories("C:/vcpkg/installed/x64-windows/include/SDL2")
link_directories("C://vcpkg/installed/x64-windows/lib")


# Add your executable
add_executable(CHIP_8 main.cpp chip8.cpp platform_layer.cpp)



# Link against SDL2 targets (these are from SDL2Config.cmake)
target_link_libraries(CHIP_8 PRIVATE SDL2)
