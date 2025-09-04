#include "config.h"
#include "app.hpp"
#include <iostream>

int main()
{
        std::cout << "Assets Path: " ASSETS_PATH << std::endl;
        kwnc::App app{};
        app.run();
        return 0;
}
