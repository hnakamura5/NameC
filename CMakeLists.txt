cmake_minimum_required(VERSION 3.16)

project(NameC C CXX)

add_library(basic_configs INTERFACE)
target_compile_features(basic_configs INTERFACE cxx_std_17)
target_include_directories(basic_configs INTERFACE
  "${PROJECT_SOURCE_DIR}/include"
)

add_subdirectory(src)
add_subdirectory(test)
