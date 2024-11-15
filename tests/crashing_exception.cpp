#include <iostream>
#include <thread>

#include <crash_handler/crash_handler.hpp>

int main() {
    try {
        return crash_handler::fork(
            []() {
                std::cerr << "crash_handler: " << "Child crashed\n";
                std::cerr << "crash_handler: " << "It's best to launch a pop up window or a notification\n";
            },
            []() {
                using namespace std::chrono_literals;

                std::cout << "Running\n";
                std::this_thread::sleep_for(1s);
                std::cout << "Doing stuff\n";

                throw 5;

                return 0;
            }
        );
    } catch (const crash_handler::Error& e) {
        std::cerr << "crash_handler: " << e.what() << "\n";
        return 1;
    }
}
