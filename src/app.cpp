#include "app.hpp"
#include "glad/gl.h"
#include "window.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace kwnc
{
App::App()
    : window{WIDTH, HEIGHT, "Kevin Craft"},
      shader{"assets/shaders/vert.glsl", "assets/shaders/frag.glsl"}, camera{},
      map{}
{
        glfwSetInputMode(window.get_glfw_window(), GLFW_CURSOR,
                         GLFW_CURSOR_DISABLED);

        glEnable(GL_DEPTH_TEST);
}
App::~App() {}

void App::run()
{
        glm::mat4 vp;

        float last_time = 0.0f;
        float current_time = 0.0f;

        float last_fps_calc = 0.0f;
        float fps = 0.0f;
        int frames = 0;

        glfwSwapInterval(0);

        map.setup(glm::vec3{0, 0, 0});

        while (!glfwWindowShouldClose(window.get_glfw_window())) {
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Calculate delta_time
                current_time = glfwGetTime();
                window.delta_time = current_time - last_time;
                last_time = current_time;

                frames++;

                if (current_time - last_fps_calc > 0.25) {
                        fps = (float)frames / (current_time - last_fps_calc);
                        frames = 0;
                        last_fps_calc = current_time;
                }

                std::cout << "FPS: " << fps << "\n";
                // std::cout << "Camera Position: " << glm::to_string(camera.position)<< "\n";

                // Update game objects
                handle_keyboard();
                camera.update(window);

                vp = get_view_projection();

                // Bind Textures
                // Use Shader
                shader.use();
                // Update Uniforms
                shader.uniform_m4("mvp", &vp[0][0]);

                // RENDER
                map.render(shader);

                glfwSwapBuffers(window.get_glfw_window());
                glfwWaitEvents();
        }
}

void App::handle_keyboard()
{
        if (glfwGetKey(window.get_glfw_window(), GLFW_KEY_ESCAPE) ==
            GLFW_PRESS) {
                glfwSetWindowShouldClose(window.get_glfw_window(), true);
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
