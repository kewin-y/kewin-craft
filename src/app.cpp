#include "app.hpp"
#include "config.h"
#include "glad/gl.h"
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>

namespace kwnc
{
App::App()
    : window{WIDTH, HEIGHT, "Kevin Craft"},
      shader{ASSETS_PATH "shaders/vert.glsl", ASSETS_PATH "shaders/frag.glsl"},
      texture{ASSETS_PATH "textures/atlas.png"}, camera{}, map{}
{
        glfwSetInputMode(window.get_glfw_window(), GLFW_CURSOR,
                         GLFW_CURSOR_DISABLED);

        glEnable(GL_DEPTH_TEST);
}

void App::run()
{
        float last_time = 0.0f;
        float current_time = 0.0f;

        float last_fps_calc = 0.0f;
        float fps = 0.0f;
        int frames = 0;

        glfwSwapInterval(0);

        map.setup(camera.position);

        shader.use();
        shader.uniform_i("v_texture", 0);

        while (!glfwWindowShouldClose(window.get_glfw_window())) {
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Calculate delta_time
                current_time = glfwGetTime();
                window.delta_time = current_time - last_time;
                last_time = current_time;

                // Calculate FPS
                frames++;

                if (current_time - last_fps_calc > 0.25) {
                        fps = (float)frames / (current_time - last_fps_calc);
                        frames = 0;
                        last_fps_calc = current_time;
                }

                // std::cout << "FPS: " << fps << "\n";

                // Update game objects
                handle_keyboard();
                camera.update(window);

#define DONT_GENERATE_NEW_CHUNKS
#ifndef DONT_GENERATE_NEW_CHUNKS
                map.update(camera.position);
#endif

                // Bind Textures
                glActiveTexture(0);
                texture.bind();

                // Use Shader
                shader.use();
                // Update Uniforms
                shader.uniform_m4("mvp", camera.get_vp_ptr());

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
} // namespace kwnc
