#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "cursor.hpp"
#include "window.hpp"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace kwnc
{
class Camera : public Cursor
{
public:
        const float FOV = 70.0f;
        const float sensitivity = 0.2f;
        const float speed = 5.5f;

        Camera();

        void update(const Window &win);
        float *get_vp_ptr();

protected:
        void cursor_position_callback(double x_pos, double y_pos) override;

private:
        glm::vec3 position{0.0f, 32.0f, 1.0f};
        glm::vec3 direction{0.0f, 0.0f, -1.0f};

        // NOTE: Given that this game is Minecraft clone, there
        // should be no reason why this vector changes
        const glm::vec3 up{0.0f, 1.0f, 0.0f};

        glm::mat4 vp;

        double last_x{0.0f}, last_y{0.0f};
        float yaw{-90.0f}, pitch{0.0f};
        bool first_mouse{true};

        void handle_keyboard(const Window &win);
        void update_vp(const Window &win);
};
} // namespace kwnc

#endif // !CAMERA_HPP
