#include "analyze.hpp"

namespace analyze
{
  Gif::Gif(const std::string& filename)
    : error(0), gif(DGifOpenFileName(filename.c_str(), &error), cleanup_gif)
  {
    // gif.reset(DGifOpenFileName(filename.c_str(), &error));

    if (error != 0)
    {
      throw std::runtime_error("Failed to open gif");
    }

    if (DGifSlurp(gif.get()) != GIF_OK)
    {
      throw std::runtime_error("Failed to slurp gif");
    }
  }

  void Gif::cleanup_gif(GifFileType *f)
  {
    int error = 0;
    DGifCloseFile(f, &error);
  }
}
