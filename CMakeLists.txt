cmake_minimum_required(VERSION 3.10)

project(kettenantrieb)

include_directories(
    .
    catch2
)

add_executable(Kettenantrieb_test
     Kettenantrieb_test.cpp
)
