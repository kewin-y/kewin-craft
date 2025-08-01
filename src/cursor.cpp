#include "cursor.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <vector>

namespace kwnc
{
std::vector<Cursor *> Cursor::instances;

Cursor::Cursor() { instances.push_back(this); }
Cursor::~Cursor()
{
        instances.erase(std::remove(instances.begin(), instances.end(), this),
                        instances.end());
}

void Cursor::setup_callback(GLFWwindow *window)
{
        glfwSetCursorPosCallback(window, _cursor_pos_callback);
}

void Cursor::_cursor_pos_callback(GLFWwindow *window, double x_pos,
                                  double y_pos)
{
        for (auto cursor : instances) {
                cursor->cursor_position_callback(x_pos, y_pos);
        }
}
} // namespace kwnc
