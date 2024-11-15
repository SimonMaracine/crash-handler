#pragma once

#include <stdexcept>
#include <string>

namespace crash_handler {
    /*
        Generally, fork should be called first in main, running the actual program, denoted by main_program,
        in the child, and the crash handler, denoted by crash_handle, in the current process.

        If the child terminates successfully, it returns from fork with an exit code. The crash handler then exits
        from fork with 0, without calling crash_handle.

        If, however, the child doesn't terminate normally, the current process calls crash_handle and exits from
        fork with 1.

        fork throws Error, if any of the system calls fail, it throws std::bad_alloc, if any memory allocation fails,
        and it throws any exception that main_program and crash_handle may throw.
    */

    // Information about the crash
    struct CrashInfo {
        std::string message;  // Platform specific message containing various information
    };

    // Crash handler signature
    using CrashHandle = void(const CrashInfo&);

    // Main program signature
    using MainProgramArgs = int(int, char**);

    // Main program signature without command line arguments
    using MainProgram = int();

    // Fork the current process, running the actual program in main_program
    int fork(CrashHandle crash_handle, MainProgramArgs main_program, int argc, char** argv);

    // Fork the current process, running the actual program in main_program; ignore command line arguments
    int fork(CrashHandle crash_handle, MainProgram main_program);

    // Error thrown when fork fails to create the process, or fails to wait for the process
    struct Error : std::runtime_error {
        explicit Error(const char* message)
            : std::runtime_error(message) {}
        explicit Error(const std::string& message)
            : std::runtime_error(message) {}
    };
}
