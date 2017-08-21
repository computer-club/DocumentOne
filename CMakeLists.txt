cmake_minimum_required(VERSION 2.8.9)

# global set bin directory
set(EXECUTABLE_OUTPUT_PATH "bin")

# main project
project(HelloWorld)
set(SOURCES cpp/HelloWorld.cpp)
add_executable(HelloWorld ${SOURCES})

# SQL Project
project(SQLTest)
execute_process(COMMAND mysql_config --cflags
 OUTPUT_VARIABLE MYSQL_CFLAGS
 OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND mysql_config --libs
 OUTPUT_VARIABLE MYSQL_LIBS
 OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MYSQL_CFLAGS} ${MYSQL_LIBS}")
set(SOURCES cpp/SQLTest.cpp cpp/Dbms.cpp cpp/SysException.cpp)
add_executable(SQLTest ${SOURCES})

# Table Test
project(TableTest)
set(SOURCES cpp/TableTest.cpp cpp/UserTables.cpp)
add_executable(TableTest ${SOURCES})
