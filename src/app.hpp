#ifndef APP_HPP
#define APP_HPP

#include "window.hpp"
#include "chunk.hpp"
#include "shader.hpp"

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
        Shader shader;
        Chunk chunk;
};
} // namespace kwnc

#endif // 1APP_HPP
