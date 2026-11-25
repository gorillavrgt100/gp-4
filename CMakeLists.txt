cmake_minimum_required(VERSION 3.20)
project(GP4Windows LANGUAGES CXX)

if(NOT WIN32)
  message(FATAL_ERROR "GP4 Windows Prototype is intended to be built on Windows.")
endif()

add_executable(GP4Windows WIN32 src/main.cpp)
target_compile_features(GP4Windows PRIVATE cxx_std_17)
target_compile_definitions(GP4Windows PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
target_link_libraries(GP4Windows PRIVATE opengl32 gdi32 user32)
