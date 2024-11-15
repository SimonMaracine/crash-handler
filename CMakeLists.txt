cmake_minimum_required(VERSION 3.20)

project("crash-handler" LANGUAGES CXX)

function(set_compile_options target)
    if(UNIX)
        target_compile_options(${target} PRIVATE "-Wall" "-Wextra" "-Wpedantic" "-Wconversion")
    endif()
endfunction()

add_subdirectory(crash_handler)
add_subdirectory(tests)
