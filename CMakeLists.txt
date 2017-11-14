cmake_minimum_required(VERSION 2.8.9)

# global set bin directory
set(EXECUTABLE_OUTPUT_PATH "bin")

# main project
# project(OpenForum)
# execute_process(COMMAND mysql_config --cflags
#  OUTPUT_VARIABLE MYSQL_CFLAGS
#  OUTPUT_STRIP_TRAILING_WHITESPACE)
# execute_process(COMMAND mysql_config --libs
#  OUTPUT_VARIABLE MYSQL_LIBS
#  OUTPUT_STRIP_TRAILING_WHITESPACE)
# set(SOURCES cpp/DatabaseUtility.cpp cpp/Dbms.cpp cpp/Column.cpp
#     cpp/User.cpp cpp/Post.cpp cpp/SysException.cpp)
# add_executable(OpenForum ${SOURCES})
# set_target_properties(OpenForum PROPERTIES COMPILE_FLAGS "${MYSQL_CFLAGS}")
# set_target_properties(OpenForum PROPERTIES LINK_FLAGS "${MYSQL_LIBS}")

# database utility
project(DatabaseUtility)
execute_process(COMMAND mysql_config --cflags
 OUTPUT_VARIABLE MYSQL_CFLAGS
 OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND mysql_config --libs
 OUTPUT_VARIABLE MYSQL_LIBS
 OUTPUT_STRIP_TRAILING_WHITESPACE)
set(SOURCES cpp/DatabaseUtility.cpp cpp/Dbms.cpp cpp/Column.cpp
    cpp/User.cpp cpp/Post.cpp cpp/SysException.cpp)
add_executable(DatabaseUtility ${SOURCES})
set_target_properties(DatabaseUtility PROPERTIES COMPILE_FLAGS "${MYSQL_CFLAGS}")
set_target_properties(DatabaseUtility PROPERTIES LINK_FLAGS "${MYSQL_LIBS}")
