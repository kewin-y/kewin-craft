#ifndef APP_HPP
#define APP_HPP

#include "window.hpp"
#include "chunk.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include <glm/glm.hpp>

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
        Camera camera;

        void handle_keyboard();
        glm::mat4 get_view_projection();
};
} // namespace kwnc

#endif // 1APP_HPP
