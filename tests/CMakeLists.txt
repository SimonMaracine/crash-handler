cmake_minimum_required(VERSION 3.20)

add_executable(simple "simple.cpp")
target_link_libraries(simple PRIVATE crash_handler)

add_executable(simple_args "simple_args.cpp")
target_link_libraries(simple_args PRIVATE crash_handler)

add_executable(crashing_abort "crashing_abort.cpp")
target_link_libraries(crashing_abort PRIVATE crash_handler)

add_executable(crashing_exception "crashing_exception.cpp")
target_link_libraries(crashing_exception PRIVATE crash_handler)

add_executable(crashing_segmentation_fault "crashing_segmentation_fault.cpp")
target_link_libraries(crashing_segmentation_fault PRIVATE crash_handler)

add_executable(crashing_abort_gui "crashing_abort_gui.cpp")
target_link_libraries(crashing_abort_gui PRIVATE crash_handler)

add_executable(simple_sigint_default "simple_sigint_default.cpp")
target_link_libraries(simple_sigint_default PRIVATE crash_handler)

add_executable(simple_sigint_handled "simple_sigint_handled.cpp")
target_link_libraries(simple_sigint_handled PRIVATE crash_handler)
