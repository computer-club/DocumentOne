cmake_minimum_required(VERSION 2.8.9)

# global set bin directory
set(EXECUTABLE_OUTPUT_PATH "bin")

# main project
project(HelloWorld)

# main add cpp files
set(SOURCES cpp/Main.cpp)

# main add executable
add_executable(application ${SOURCES})
