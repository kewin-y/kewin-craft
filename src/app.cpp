#include "app.hpp"
#include "glad/gl.h"
#include "window.hpp"
#include <GLFW/glfw3.h>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

namespace kwnc
{
App::App() : window{WIDTH, HEIGHT, "Kevin Craft"} {}
App::~App() {}

void App::run()
{
        while (!glfwWindowShouldClose(window.self)) {
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // RENDER
                glfwSwapBuffers(window.self);
                glfwWaitEvents();
        }
}
} // namespace kwnc
