#include "assembly.hpp"
#include "analyze.hpp"
#include "gl.hpp"

namespace assembly
{
  std::unique_ptr<analyze::Gif> gif;

  void go(const std::string& filename)
  {
    gif.reset(new analyze::Gif(filename));

    std::cout << "Loaded " << filename << std::endl;
    auto g = gif.get()->gif.get();

    gl::set_texture(g->SWidth, g->SHeight, g->SavedImages[0].RasterBits);
  }
}
