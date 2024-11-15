#include <iostream>
#include <thread>
#include <csignal>

#include <crash_handler/crash_handler.hpp>

static volatile sig_atomic_t interrupted {0};

extern "C" void interrupt(int) {
    interrupted = 1;
}

static int process_start() {
    using namespace std::chrono_literals;

    if (std::signal(SIGINT, interrupt) == SIG_ERR) {
        std::cerr << "Could not setup interrupt handler\n";
        return 1;
    }

    std::cout << "Running\n";

    while (true) {
        std::this_thread::sleep_for(1s);
        std::cout << "Doing stuff\n";

        if (interrupted == 1) {
            break;
        }
    }

    std::cout << "Done\n";

    return 0;
}

static void crash_handle() {
    std::cerr << "crash_handler: " << "Child crashed\n";
    std::cerr << "crash_handler: " << "It's best to launch a pop up window or a notification\n";
}

int main() {
    if (std::signal(SIGINT, SIG_IGN) == SIG_ERR) {
        std::cerr << "crash_handler: " << "Could not setup interrupt handler\n";
        return 1;
    }

    try {
        return crash_handler::fork(crash_handle, process_start);
    } catch (const crash_handler::Error& e) {
        std::cerr << "crash_handler: " << e.what() << "\n";
        return 1;
    }
}
