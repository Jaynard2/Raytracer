cmake_minimum_required(VERSION 3.26)
project(raytracer)
add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED TRUE)
target_compile_options(${PROJECT_NAME} PUBLIC -march=native)
target_include_directories(${PROJECT_NAME} PRIVATE include)
add_subdirectory(src)

include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY git@github.com:fmtlib/fmt.git
    GIT_TAG 11.0.2
)
FetchContent_MakeAvailable(fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
