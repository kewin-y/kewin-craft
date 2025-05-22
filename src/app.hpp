#ifndef APP_HPP
#define APP_HPP

#include "window.hpp"

namespace kwnc
{
class App
{
public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        App();
        ~App();

        void run();

private:
        Window window;
};
} // namespace kwnc

#endif // 1APP_HPP
