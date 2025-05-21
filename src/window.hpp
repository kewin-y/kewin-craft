#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include <string>

namespace kwnc
{
class Window
{
public:
        int width;
        int height;
        std::string name;

        Window(int w, int h, const std::string &n);
        ~Window();

        GLFWwindow *get_glfw_window();

private:
        GLFWwindow *glfw_window;

        static void fb_size_callback(GLFWwindow *win, int width, int height);
};
} // namespace kwnc

#endif // WINDOW_HPP
