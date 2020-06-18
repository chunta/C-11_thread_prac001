#include <iostream>
#include <thread>

void hello() {
    for (int i = 0; i < 10; i++) {
        std::cout << "Hello Concurrent World\n";
    }
}

int main() {
    std::thread t(hello);
    //t.join();
    return 1;
}
