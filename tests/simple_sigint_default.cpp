#include <iostream>
#include <thread>
#include <csignal>

#include <crash_handler/crash_handler.hpp>

int main() {
    if (std::signal(SIGINT, SIG_IGN) == SIG_ERR) {
        std::cerr << "crash_handler: " << "Could not setup interrupt handler\n";
        return 1;
    }

    try {
        return crash_handler::fork(
            [](const crash_handler::CrashInfo& info) {
                std::cerr << "crash_handler: " << "Child crashed: " << info.message << '\n';
                std::cerr << "crash_handler: " << "It's best to launch a pop up window or a notification\n";
            },
            []() {
                using namespace std::chrono_literals;

                if (std::signal(SIGINT, SIG_DFL) == SIG_ERR) {
                    std::cerr << "Could not setup interrupt handler\n";
                    return 1;
                }

                std::cout << "Running\n";

                for (int i {0}; i < 5; i++) {
                    std::this_thread::sleep_for(1s);
                    std::cout << "Doing stuff\n";
                }

                std::cout << "Done\n";

                return 0;
            }
        );
    } catch (const crash_handler::Error& e) {
        std::cerr << "crash_handler: " << e.what() << '\n';
        return 1;
    }
}
