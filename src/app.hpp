#ifndef APP_HPP
#define APP_HPP

#include "texture.hpp"
#include "window.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "map.hpp"
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
        Texture texture;
        Camera camera;
        Map map;

        void handle_keyboard();
        glm::mat4 get_view_projection();
};
} // namespace kwnc

#endif // !APP_HPP
