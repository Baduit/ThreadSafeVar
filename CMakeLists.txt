cmake_minimum_required(VERSION 3.0)

project(ThreadSafeVar)
add_definitions(-std=c++17)
set(SRCS
src/main.cpp
)
include_directories(ThreadSafeVar PRIVATE ./src ./test)
add_executable(ThreadSafeVar ${SRCS})
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ThreadSafeVar DESTINATION bin)
