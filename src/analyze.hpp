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

  private:
    static void cleanup_gif(GifFileType *f);

    std::vector<int> frames{};
  };
}
