cmake_minimum_required(VERSION 3.20)

add_library(crash_handler STATIC
    "include/crash_handler/crash_handler.hpp"
    "src/crash_handler.cpp"
)

target_include_directories(crash_handler PUBLIC "include")
