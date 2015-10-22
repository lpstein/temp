#include "precompiled.hpp"
#include "gl.hpp"

using namespace std;

void glfw_error_callback(int error, const char *description)
{
  cout << "GLFW Error " << error << ": " << description << endl;
}

int main(int argc, char** argv)
{
  cout << "Booting up GLFW" << endl;

  if (!glfwInit())
  {
    throw runtime_error("Failed to initialize GLFW");
  }
  glfwSetErrorCallback(glfw_error_callback);

  // GLFW config
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window
  GLFWwindow *window = glfwCreateWindow(800, 800, "gif of the magic", nullptr, nullptr);
  if (window == nullptr)
  {
    throw runtime_error("Failed to create GLFW window");
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  cout << "Window ready" << endl;
  cout << "Rollin' with GL" << glGetString(GL_VERSION) << endl;

  // Figure out sizing n stuff
  int width_points, height_points;
  int width_pixels, height_pixels;
  glfwGetWindowSize(window, &width_points, &height_points);
  glfwGetFramebufferSize(window, &width_pixels, &height_pixels);

  // GL setup
  gl::init(width_pixels, height_pixels, (float)width_pixels / (float)width_points);

  // Main loop
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl::render();
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
