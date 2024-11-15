#include "crash_handler/crash_handler.hpp"

#include <utility>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/wait.h>

// https://man7.org/linux/man-pages/man2/fork.2.html
// https://www.man7.org/linux/man-pages/man2/wait.2.html
// https://man7.org/linux/man-pages/man7/signal.7.html
// https://man7.org/linux/man-pages/man3/strerror.3.html
// https://man7.org/linux/man-pages/man3/strsignal.3.html
// https://www.man7.org/linux/man-pages/man3/errno.3.html

namespace crash_handler {
    template<typename F, typename... Args>
    static int fork_base(CrashHandle crash_handle, F&& main_program, Args&&... args) {
        using namespace std::literals;

        const pid_t fresult {::fork()};

        if (fresult == 0) {
            return main_program(std::forward<Args>(args)...);
        } else if (fresult > 0) {
            int wstatus {};

            // Wait only for terminated child
            // waitpid doesn't conform to POSIX and it does exactly what we want
            if (waitpid(fresult, &wstatus, 0) < 0) {
                throw Error("Could not wait for process: "s + std::strerror(errno));
            }

            if (!WIFEXITED(wstatus)) {
                CrashInfo info;

                // Better to give up on message than to never call crash_handle
                try {
                    if (WIFSIGNALED(wstatus)) {
                        info.message = "Process terminated by signal "s + sigabbrev_np(WTERMSIG(wstatus));
#ifdef WCOREDUMP
                        if (WCOREDUMP(wstatus)) {
                            info.message += " and produced a core dump";
                        } else {
                            info.message += "; produced no core dump";
                        }
#endif
                    } else {
                        info.message = "Process terminated";
                    }
                } catch (const std::bad_alloc&) {}

                crash_handle(info);

                return 1;
            }

            return 0;
        } else {
            throw Error("Could not create process: "s + std::strerror(errno));
        }
    }

    int fork(CrashHandle crash_handle, MainProgramArgs main_program, int argc, char** argv) {
        return fork_base(crash_handle, main_program, argc, argv);
    }

    int fork(CrashHandle crash_handle, MainProgram main_program) {
        return fork_base(crash_handle, main_program);
    }
}
