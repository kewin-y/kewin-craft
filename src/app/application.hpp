#ifndef APP_HPP
#define APP_HPP

#include "gameplay/camera.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "platform/window.hpp"
#include "world/world.hpp"
#include <glm/glm.hpp>

namespace kwnc
{
class App
{
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  App();

  void run();

private:
  Window window;
  const Shader shader;
  const Texture texture;
  Camera camera;
  World map;

  void handle_keyboard();
};
} // namespace kwnc

#endif // !APP_HPP
