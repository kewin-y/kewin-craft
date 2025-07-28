#include "camera.hpp"
#include <glm/geometric.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

namespace kwnc
{
Camera::Camera() : Cursor() {}

void Camera::update(const Window &win)
{
        handle_keyboard(win);
        update_vp(win);
}

float *Camera::get_vp_ptr() { return &vp[0][0]; }

void Camera::cursor_position_callback(double x_pos, double y_pos)
{
        double x_offset, y_offset;

        if (first_mouse) {
                last_x = x_pos;
                last_y = y_pos;
                first_mouse = false;
        }

        x_offset = x_pos - last_x;
        y_offset = last_y - y_pos;

        last_x = x_pos;
        last_y = y_pos;

        yaw += x_offset * sensitivity;
        pitch += y_offset * sensitivity;

        if (pitch > 89.0f)
                pitch = 89.0f;
        if (pitch < -89.0f)
                pitch = -89.0f;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction = glm::normalize(direction);
};

void Camera::handle_keyboard(const Window &win)
{
        glm::vec3 forward, right;

        right = glm::normalize(glm::cross(direction, up));
        forward = glm::normalize(glm::cross(up, right));

        if (glfwGetKey(win.get_glfw_window(), GLFW_KEY_W) == GLFW_PRESS) {
                position += (speed * win.delta_time) * forward;
        }

        if (glfwGetKey(win.get_glfw_window(), GLFW_KEY_S) == GLFW_PRESS) {
                position += (-speed * win.delta_time) * forward;
        }

        if (glfwGetKey(win.get_glfw_window(), GLFW_KEY_A) == GLFW_PRESS) {
                position += (-speed * win.delta_time) * right;
        }

        if (glfwGetKey(win.get_glfw_window(), GLFW_KEY_D) == GLFW_PRESS) {
                position += (speed * win.delta_time) * right;
        }

        if (glfwGetKey(win.get_glfw_window(), GLFW_KEY_SPACE) == GLFW_PRESS) {
                position += (speed * win.delta_time) * up;
        }

        if (glfwGetKey(win.get_glfw_window(), GLFW_KEY_LEFT_SHIFT) ==
            GLFW_PRESS) {
                position += (-speed * win.delta_time) * up;
        }
}

void Camera::update_vp(const Window &win)
{
        glm::mat4 view;
        glm::mat4 projection;

        view = glm::lookAt(position, position + direction, up);
        projection =
            glm::perspective(glm::radians(FOV), win.aspect, 0.1f, 100.0f);

        vp = projection * view;
}
} // namespace kwnc
