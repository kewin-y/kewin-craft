#include "app.hpp"
#include "glad/gl.h"
#include "window.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace kwnc
{
App::App()
    : window{WIDTH, HEIGHT, "Kevin Craft"},
      shader{"assets/shaders/vert.glsl", "assets/shaders/frag.glsl"},
      chunk{0, 0, 0}, camera{}
{
        chunk.generate_mesh();

        glfwSetInputMode(window.get_glfw_window(), GLFW_CURSOR,
                         GLFW_CURSOR_DISABLED);

        glEnable(GL_DEPTH_TEST);
}
App::~App() {}

void App::run()
{
        glm::mat4 vp;
        float last_frame = 0.0f;
        float current_frame = 0.0f;

        while (!glfwWindowShouldClose(window.get_glfw_window())) {
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Calculate delta_time
                current_frame = glfwGetTime();
                window.delta_time = current_frame - last_frame;
                last_frame = current_frame;

                // Update game objects
                handle_keyboard();
                camera.update(window);

                vp = get_view_projection();

                // Bind Textures
                // Use Shader
                shader.use();
                // Update Uniforms
                shader.uniform_m4("mvp", &vp[0][0]);
                shader.uniform_v3("world_pos", &chunk.position[0]);

                chunk.vertex_array.draw();

                // RENDER
                glfwSwapBuffers(window.get_glfw_window());
                glfwWaitEvents();
        }
}

void App::handle_keyboard()
{
        if (glfwGetKey(window.get_glfw_window(), GLFW_KEY_ESCAPE) ==
            GLFW_PRESS) {
                glfwSetWindowShouldClose(window.get_glfw_window(), 1);
        }
}

glm::mat4 App::get_view_projection()
{
        glm::mat4 view;
        glm::mat4 projection;

        view = glm::lookAt(camera.position, camera.target, camera.up);
        projection = glm::perspective(glm::radians(camera.FOV), window.aspect,
                                      0.1f, 100.0f);

        return projection * view;
}
} // namespace kwnc
