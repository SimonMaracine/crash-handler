#include "crash_handler/crash_handler.hpp"

#include <utility>

#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/wait.h>

namespace crash_handler {
    template<typename F, typename... Args>
    static int fork_generic(CrashHandle crash_handle, F&& process_start, Args&&... args) {
        using namespace std::literals;

        const pid_t fresult {::fork()};

        if (fresult == 0) {
            return process_start(std::forward<Args>(args)...);
        } else if (fresult > 0) {
            int wstatus {};
            const int wresult {waitpid(fresult, &wstatus, 0)};

            if (wresult < 0) {
                // TODO terminate child
                throw Error("Could not wait for process: "s + std::strerror(errno));
            }

            if (!WIFEXITED(wstatus)) {
                crash_handle();
                return 1;
            }

            return 0;
        } else {
            throw Error("Could not create process: "s + std::strerror(errno));
        }
    }

    int fork(CrashHandle crash_handle, ProcessStartArgs process_start, int argc, char** argv) {
        return fork_generic(crash_handle, process_start, argc, argv);
    }

    int fork(CrashHandle crash_handle, ProcessStart process_start) {
        return fork_generic(crash_handle, process_start);
    }
}
