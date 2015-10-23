#include "precompiled.hpp"

namespace analyze
{
  class Gif
  {
  private:
    int error;

  public:
    Gif(const std::string& filename);

    std::unique_ptr<GifFileType, std::function<void(GifFileType*)>> gif;

    // Returns a frame as a good ol' 32bpp bitmap
    std::vector<unsigned char> get_frame(int id);

    int frames = 0;
    int width = 0;
    int height = 0;

  private:
    static void cleanup_gif(GifFileType *f);
    int transparency_index = -1;
  };
}
