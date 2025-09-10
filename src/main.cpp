#include "config.h"
#include "app.hpp"
#include <iostream>
#include <thread>

int main()
{
        std::cout << "Main thread: " << std::this_thread::get_id() << std::endl;
        std::cout << "Assets Path: " ASSETS_PATH << std::endl;
        kwnc::App app{};
        app.run();
        return 0;
}
