#include "precompiled.hpp"

namespace analyze
{
  class Gif
  {
  public:
    Gif(const std::string& filename);

  private:
    static void cleanup_gif(GifFileType *f);

    int error = 0;
    std::unique_ptr<GifFileType, std::function<void(GifFileType*)>> gif;
    std::vector<int> frames{};
  };
}
