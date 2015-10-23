#include "assembly.hpp"
#include "analyze.hpp"
#include "gl.hpp"

namespace assembly
{
  std::unique_ptr<analyze::Gif> gif;
  int cur_frame = 0;
  bool running = true;
  std::thread timer;

  void go(const std::string& filename)
  {
    gif.reset(new analyze::Gif(filename));

    std::cout << "Loaded " << filename << std::endl;
    auto g = gif.get()->gif.get();

    timer = std::thread([&]()
    {
      while (running)
      {
        tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(70));
      }
    });
  }

  void tick()
  {
    gl::set_texture(gif->width, gif->height, gif->get_frame(cur_frame));
    cur_frame++;
    if (cur_frame > gif->frames - 1)
    {
      cur_frame = 0;
    }
  }

  void stop()
  {
    running = false;
    timer.join();
  }
}
