#pragma once

#include <stdexcept>
#include <string>

namespace crash_handler {
    using CrashHandle = void();
    using ProcessStartArgs = int(int, char**);
    using ProcessStart = int();

    int fork(CrashHandle crash_handle, ProcessStartArgs process_start, int argc, char** argv);
    int fork(CrashHandle crash_handle, ProcessStart process_start);

    struct Error : std::runtime_error {
        explicit Error(const char* message)
            : std::runtime_error(message) {}
        explicit Error(const std::string& message)
            : std::runtime_error(message) {}
    };
}
