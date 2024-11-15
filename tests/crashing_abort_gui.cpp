#include <iostream>
#include <thread>
#include <cstdlib>

#include <crash_handler/crash_handler.hpp>

int main() {
    try {
        return crash_handler::fork(
            [](const crash_handler::CrashInfo& info) {
                // Works only for GTK
                const auto command {"zenity --error --text='Oops! The program crashed. " + info.message + ".' --title='Program Crash'"};
                const int result {std::system(command.c_str())};

                if (WEXITSTATUS(result) != 0) {
                    std::cerr << "crash_handler: Could not display error message pop up\n";
                    std::cerr << "crash_handler: " << "Child crashed: " << info.message << '\n';
                }
            },
            []() {
                using namespace std::chrono_literals;

                std::cout << "Running\n";
                std::this_thread::sleep_for(1s);
                std::cout << "Doing stuff\n";

                std::abort();

                return 0;
            }
        );
    } catch (const crash_handler::Error& e) {
        std::cerr << "crash_handler: " << e.what() << "\n";
        return 1;
    }
}
