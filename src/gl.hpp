namespace gl
{
  // Framebuffer width/height in pixels, scale factor is pixels per point
  void init(int w, int h, float scale);
  void render();
  void set_texture(int w, int h, std::vector<unsigned char> data);

  unsigned int compile_shader(unsigned int type, std::string& source);
  unsigned int compile_program(unsigned int vertex, unsigned int fragment);
}
