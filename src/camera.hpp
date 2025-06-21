#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "window.hpp"
#include <glm/glm.hpp>

namespace kwnc
{
class Camera
{
public:
        glm::vec3 position{0.0f, 0.0f, 1.0f};
        glm::vec3 direction{0.0f, 0.0f, -0.1f};
        glm::vec3 target{0.0f, 0.0f, 0.9f};

        // NOTE: Given that this game is Minecraft clone, there
        // should be no reason why this vector changes
        const glm::vec3 up{0.0f, 1.0f, 0.0f};

        const float FOV = 45.0f;
        const float sensitivity = 0.2f;
        const float speed = 2.5f;

        Camera();
        ~Camera();

        void update(const Window &win);

private:
        double last_x{0.0f}, last_y{0.0f};
        float pitch{0.0f}, yaw{-90.0f};

        void handle_keyboard(const Window &win);
};
} // namespace kwnc

#endif // !CAMERA_HPP
