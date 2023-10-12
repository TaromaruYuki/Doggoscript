cmake_minimum_required(VERSION 3.22)
project(Doggoscript VERSION 0.3.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/main.cpp res/logo/doggoscript.rc)

add_subdirectory(doggoscript)
target_include_directories(${PROJECT_NAME} PRIVATE doggoscript/include)
target_link_libraries(${PROJECT_NAME} PRIVATE doggoscript)