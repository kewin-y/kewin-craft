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
        float aspect;
        std::string name;

        float delta_time{0.0f};

        Window(int w, int h, const std::string &n);
        ~Window();

        GLFWwindow *get_glfw_window() const;
        int get_first_mouse() const;
        void mouse_entered();

private:
        GLFWwindow *glfw_window;
        int first_mouse{1};

        static void fb_size_callback(GLFWwindow *win, int width, int height);
};
} // namespace kwnc

#endif // !WINDOW_HPP
