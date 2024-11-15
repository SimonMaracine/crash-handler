# crash-handler

This is a little C++ library made just for fun, that adds a crash handling mechanism to an executable.

It only works on Linux.

## Usage

```txt
git submodule add https://github.com/SimonMaracine/crash-handler <path/to/submodule>
```

```cmake
add_subdirectory(<path/to/submodule>)
target_link_libraries(<target> PRIVATE crash_handler)
```

```c++
#include <iostream>
#include <thread>
#include <cstdlib>

#include <crash_handler/crash_handler.hpp>

int main() {
    try {
        return crash_handler::fork(
            [](const crash_handler::CrashInfo& info) {
                std::cerr << "crash_handler: " << "Child crashed: " << info.message << '\n';
            },
            []() {
                using namespace std::chrono_literals;

                std::cout << "Running\n";
                std::this_thread::sleep_for(1s);

                std::cout << "Oh, no\n";
                std::abort();  // Crash

                std::cout << "Nevermind\n";

                return 0;
            }
        );
    } catch (const crash_handler::Error& e) {
        std::cerr << "crash_handler: " << e.what() << '\n';
        return 1;
    }
}
```
