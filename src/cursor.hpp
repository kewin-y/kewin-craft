#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include <vector>

namespace kwnc
{
class Cursor
{
public:
        Cursor();

        static void setup_callback(GLFWwindow *window);

protected:
        virtual void cursor_position_callback(double x_pos, double y_pos) = 0;

private:
        static std::vector<Cursor *> instances;
        static void _cursor_pos_callback(GLFWwindow *window, double x_pos,
                                        double y_pos);
};
} // namespace kwnc

#endif // !CURSOR_HPP
