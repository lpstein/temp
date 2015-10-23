#include "precompiled.hpp"
#include "gl.hpp"

static std::string frag =
"#version 410\n"
"uniform sampler2D tex;\n"
"in vec4 vertex_color;\n"
"in vec2 uv;\n"
"out vec4 fragment_color;\n"
"\n"
"void main()\n"
"{\n"
// "   fragment_color = vertex_color;\n"
"   float l = texture(tex, uv).r;\n"
"   fragment_color = texture(tex, uv);\n"
"   if (fragment_color.w != 1.0f) { discard; }\n"
// "   fragment_color = vec4(l, l, l, 1.0f);\n"
// "  fragment_color = vec4(uv, 0.0f, 1.0f);\n"
"}\n"
"";

static std::string vert =
"#version 410\n"
"\n"
"layout(location = 0) in vec2 location;\n"
"out vec4 vertex_color;\n"
"out vec2 uv;\n"
"\n"
"void main()\n"
"{\n"
"  vertex_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"  gl_Position = vec4(location, 0.0f, 1.0f);\n"
"  uv = location + 0.5f;\n"
"  uv.y = 1.0f - uv.y;\n" // Flip UV due to ogl have weird coord system
"}\n"
"";

namespace gl
{
  unsigned int texture = -1;
  unsigned int vao = -1;
  unsigned int vbo = -1;
  unsigned int tex_loc = -1;

  bool must_set_texture = false;
  int tex_width = 0;
  int tex_height = 0;
  std::vector<unsigned char> tex_data;

  void actually_set_texture()
  {
    GLenum err = GL_NO_ERROR;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data.data());
    while ((err = glGetError()) != GL_NO_ERROR)
    {
      std::cout << "b: " << gluErrorString(err) << std::endl;
    }

    must_set_texture = false;
  }

  void init(int w, int h, float scale)
  {
    glViewport(0, 0, w, h);
    glFrontFace(GL_CW);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float scale_x = 1.0f;
    float scale_y = 1.0f;
    if (w > h) {
      scale_y = float(w) / float(h);
    } else if (h < w) {
      scale_x = float(h) / float(w);
    }
    float data[] = {
      scale_x * -0.5f, scale_y * -0.5f,
      scale_x * +0.5f, scale_y * -0.5f,
      scale_x * +0.5f, scale_y * +0.5f,
      scale_x * -0.5f, scale_y * +0.5f
    };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

    int vertex_shader = compile_shader(GL_VERTEX_SHADER, vert);
    int fragment_shader = compile_shader(GL_FRAGMENT_SHADER, frag);
    int program = compile_program(vertex_shader, fragment_shader);
    tex_loc = glGetUniformLocation(program, "tex");
    glUseProgram(program);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }

  void render()
  {
    GLenum err = GL_NO_ERROR;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
      std::cout << "r: " << gluErrorString(err) << std::endl;
    }

    if (must_set_texture)
    {
      actually_set_texture();
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(tex_loc, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  }

  // This ain't threadsafe on ARM
  void set_texture(int w, int h, std::vector<unsigned char>data)
  {
    tex_width = w;
    tex_height = h;
    tex_data = data;
    must_set_texture = true;
  }

  unsigned int compile_shader(unsigned int type, std::string& source)
  {
    auto shader = glCreateShader(type);
    const char* bytes = source.c_str();
    glShaderSource(shader, 1, &bytes, nullptr);
    glCompileShader(shader);

    // If we failed, fetch the error log
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
      int size;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);

      auto log = std::make_unique<char*>(new char[size]);
      glGetShaderInfoLog(shader, size, nullptr, *log.get());

      glDeleteShader(shader);
      throw std::runtime_error(std::string(*log.get()));
    }

    // Otherwise just return the shader itself
    return shader;
  }

  unsigned int compile_program(unsigned int vertex, unsigned int fragment)
  {
    auto program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    // Check for failure
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
      int size;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);

      auto log = std::make_unique<char*>(new char[size]);
      glGetProgramInfoLog(program, size, nullptr, *log.get());

      glDeleteProgram(program);
      throw std::runtime_error(std::string(*log.get()));
    }

    return program;
  }
}
