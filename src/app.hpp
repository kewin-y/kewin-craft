#ifndef APP_HPP
#define APP_HPP

#include "window.hpp"

namespace kwnc
{
class App
{
        public:
                App();
                ~App();

                void run();
        private:
                Window window;
};
} // namespace kwnc

#endif // 1APP_HPP
