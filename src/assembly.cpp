#include "assembly.hpp"
#include "analyze.hpp"

namespace assembly
{
  std::unique_ptr<analyze::Gif> gif;

  void go(const std::string& filename)
  {
    gif.reset(new analyze::Gif(filename));

    std::cout << "Loaded " << filename << std::endl;
  }
}
