#include "app/application.hpp"
#include "config.h"
#include <iostream>
#include <thread>

int main()
{
        std::cout << "Assets Path: " ASSETS_PATH << std::endl;
        kwnc::App app{};
        app.run();
        return 0;
}
