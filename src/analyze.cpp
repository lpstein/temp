#include "analyze.hpp"

namespace analyze
{
  template <typename T>
  int get_transparency_info(T *thing)
  {
    int transparent_index = -1;
    for (int i=0; i<thing->ExtensionBlockCount; i++)
    {
      auto block = &thing->ExtensionBlocks[i];
      if (block->Function == GRAPHICS_EXT_FUNC_CODE)
      {
        transparent_index = block->Bytes[3];
      }
    }

    return transparent_index;
  }

  Gif::Gif(const std::string& filename)
    : error(0), gif(DGifOpenFileName(filename.c_str(), &error), cleanup_gif)
  {
    if (error != 0)
    {
      throw std::runtime_error("Failed to open gif");
    }

    if (DGifSlurp(gif.get()) != GIF_OK)
    {
      throw std::runtime_error("Failed to slurp gif");
    }

    frames = gif->ImageCount;
    width = gif->SWidth;
    height = gif->SHeight;

    // Look for transparency info
    transparency_index = get_transparency_info(gif.get());
  }

  void Gif::cleanup_gif(GifFileType *f)
  {
    int error = 0;
    DGifCloseFile(f, &error);
  }

  std::vector<unsigned char> Gif::get_frame(int id)
  {

    // Yank out the right image
    if (id > gif->ImageCount)
    {
      id = gif->ImageCount;
    }
    ColorMapObject *map = gif->SColorMap;
    auto img = &gif->SavedImages[id];
    if (map == nullptr)
    {
      map = img->ImageDesc.ColorMap;
    }

    // Look for transparency info
    transparency_index = get_transparency_info(img);

    // Read the data itself
    std::vector<unsigned char> data(4 * gif->SWidth * gif->SHeight);
    for (int y=0; y<gif->SHeight; y++)
    {
      for (int x=0; x<gif->SWidth; x++)
      {
        int i = (y * gif->SWidth + x);
        int di = i * 4;
        int v = img->RasterBits[i];

        if (transparency_index >= 0 && v == transparency_index)
        {
          data[di] = 255;
          data[di + 1] = 0;
          data[di + 2] = 0;
          data[di + 3] = 0;
        }
        else
        {

          auto color = &map->Colors[v];
          data[di] = color->Red;
          data[di + 1] = color->Green;
          data[di + 2] = color->Blue;
          data[di + 3] = 255;
        }
      }
    }

    return data;
  }
}
