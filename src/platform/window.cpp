#include "platform/window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

namespace kwnc
{
Window::Window(int w, int h, const std::string &n)
    : width{w}, height{h}, name{n}, delta_time{0.0f}
{
  aspect = (float)width / (float)height;

  glfwSetErrorCallback([](int, const char *desc) {
    std::cerr << desc << "\n";
    std::exit(EXIT_FAILURE);
  });
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfw_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

  if (nullptr == glfw_window) {
    std::cerr << "Failed to create window\n";
    glfwTerminate();
    exit(1);
  }

  glfwMakeContextCurrent(glfw_window);
  glfwSetWindowUserPointer(glfw_window, this);
  glfwSetFramebufferSizeCallback(glfw_window, _fb_size_cb);
  glfwSetCursorPosCallback(glfw_window, _cursor_pos_cb);

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

void Window::fb_size_cb(int width, int height)
{
  glViewport(0, 0, width, height);
  this->width = width;
  this->height = height;
  this->aspect = (float)width / (float)height;
}

void Window::set_cursor_pos_cb(std::function<void(double, double)> cb)
{
  // Cast `cb` to rvalue with `std::move`, `cursor_pos_cb`'s move constructor is
  // called
  cursor_pos_cb = std::move(cb);
}

GLFWwindow *Window::get_glfw_window() const { return glfw_window; }

void Window::_fb_size_cb(GLFWwindow *win, int width, int height)
{
  Window *window = reinterpret_cast<Window *>(glfwGetWindowUserPointer(win));
  window->fb_size_cb(width, height);
}

void Window::_cursor_pos_cb(GLFWwindow *win, double x_pos, double y_pos)
{
  Window *window = reinterpret_cast<Window *>(glfwGetWindowUserPointer(win));
  if (window->cursor_pos_cb) {
    window->cursor_pos_cb(x_pos, y_pos);
  }
}
} // namespace kwnc
