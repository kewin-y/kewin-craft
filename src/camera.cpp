#include "camera.hpp"
#include "window.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

namespace kwnc
{
Camera::Camera() {}

Camera::~Camera() {}

void Camera::update(const Window &win)
{
        double cursor_x, cursor_y;
        double x_offset, y_offset;

        handle_keyboard(win);

        glfwGetCursorPos(win.get_glfw_window(), &cursor_x, &cursor_y);

        x_offset = cursor_x - last_x;
        y_offset = last_y - cursor_y;

        last_x = cursor_x;
        last_y = cursor_y;

        yaw += x_offset * sensitivity;
        pitch += y_offset * sensitivity;

        if (pitch > 89.0f)
                pitch = 89.0f;
        if (pitch < -89.0f)
                pitch = -89.0f;

        direction.x =
            std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        direction.y = std::sin(glm::radians(pitch));
        direction.z =
            std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

        target = position + direction;
}

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

        if (glfwGetKey(win.get_glfw_window(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                position += (-speed * win.delta_time) * up;
        }

}
} // namespace kwnc
