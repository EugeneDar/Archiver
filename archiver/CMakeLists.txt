cmake_minimum_required(VERSION 3.16)
project(archiver)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(archiver
        main.cpp
        h_tree.cpp archiver.cpp entities/table_element.cpp file_recorders/file_reader.cpp file_recorders/file_writer.cpp entities/node.cpp entities/node.cpp entities/values.h)

add_executable(test_archiver
        test_archiver.cpp
        archiver.cpp
        h_tree.cpp entities/table_element.cpp file_recorders/file_reader.cpp file_recorders/file_writer.cpp entities/node.cpp)

target_link_libraries(test_archiver ${GTEST_LIBRARIES} pthread)