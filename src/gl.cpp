#include "precompiled.hpp"
#include "gl.hpp"

namespace gl
{
  void init(int w, int h, float scale)
  {
    glViewport(0, 0, w, h);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  }

  void render()
  {
    // TODO - Something significant with my life
  }
}
