#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include <functional>
#include <string>

namespace kwnc
{
class Window
{
public:
  int width;
  int height;
  float aspect;
  float delta_time;

  std::string name;

  Window(int w, int h, const std::string &n);
  ~Window();

  void set_cursor_pos_cb(std::function<void(double, double)> cb);
  GLFWwindow *get_glfw_window() const;

private:
  GLFWwindow *glfw_window;
  std::function<void(double, double)> cursor_pos_cb;

  // Hardcoded framebuffer size callback because it will probably stay like this
  void fb_size_cb(int width, int height);

  static void _fb_size_cb(GLFWwindow *win, int width, int height);
  static void _cursor_pos_cb(GLFWwindow *win, double x_pos, double y_pos);
};
} // namespace kwnc

#endif // !WINDOW_HPP
