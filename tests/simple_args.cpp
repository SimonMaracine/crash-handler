#include <iostream>
#include <thread>

#include <crash_handler/crash_handler.hpp>

int main(int argc, char** argv) {
    try {
        return crash_handler::fork(
            [](const crash_handler::CrashInfo& info) {
                std::cerr << "crash_handler: " << "Child crashed: " << info.message << '\n';
                std::cerr << "crash_handler: " << "It's best to launch a pop up window or a notification\n";
            },
            [](int argc, char**) {
                using namespace std::chrono_literals;

                std::cout << "argc: " << argc << '\n';
                std::cout << "Running\n";

                for (int i {0}; i < 5; i++) {
                    std::this_thread::sleep_for(1s);
                    std::cout << "Doing stuff\n";
                }

                std::cout << "Done\n";

                return 0;
            },
            argc,
            argv
        );
    } catch (const crash_handler::Error& e) {
        std::cerr << "crash_handler: " << e.what() << '\n';
        return 1;
    }
}

