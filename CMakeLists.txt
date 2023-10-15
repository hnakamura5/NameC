cmake_minimum_required(VERSION 3.25)

project(NameC C CXX)

add_library(basic_configs INTERFACE)
target_compile_features(basic_configs INTERFACE cxx_std_17)
target_include_directories(basic_configs INTERFACE
  "${PROJECT_SOURCE_DIR}/include"
)

add_subdirectory(src)
add_subdirectory(test)

install(TARGETS NameC RUNTIME
        DESTINATION "bin")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h")

add_subdirectory(doc)
