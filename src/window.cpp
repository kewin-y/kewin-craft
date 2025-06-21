#include "window.hpp"
#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

namespace kwnc
{
Window::Window(int w, int h, const std::string &n)
    : width{w}, height{h}, name{n}
{
        aspect = (float)width / (float)height;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfw_window =
            glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

        if (nullptr == glfw_window) {
                std::cerr << "Failed to create window\n";
                glfwTerminate();
                exit(1);
        }

        glfwMakeContextCurrent(glfw_window);
        glfwSetWindowUserPointer(glfw_window, this);
        glfwSetFramebufferSizeCallback(glfw_window, fb_size_callback);

        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
                std::cerr << "Failed to load GLAD\n";
                glfwDestroyWindow(glfw_window);
                glfwTerminate();
                exit(1);
        }
}

Window::~Window()
{
        glfwDestroyWindow(glfw_window);
        glfwTerminate();
}

GLFWwindow *Window::get_glfw_window() const { return glfw_window; }

int Window::get_first_mouse() const { return first_mouse; }

void Window::mouse_entered() { first_mouse = 0; }

void Window::fb_size_callback(GLFWwindow *win, int width, int height)
{
        Window *window =
            reinterpret_cast<Window *>(glfwGetWindowUserPointer(win));
        glViewport(0, 0, width, height);

        window->width = width;
        window->height = height;
        window->aspect = (float)width / (float)height;
}
} // namespace kwnc
