
#ifndef _SPRITES_H
#define _SPRITES_H

#include "Surface.h"
#include "Sprite.h"
#include "types.h"


#include <vector>
#include <string>
#include <map>
#include <cstdint>

namespace Ironhead
{
namespace Graphics
{

class Sprites
{
public:
  Sprites();
  ~Sprites();

  bool init();
  void close();
  void flushSheets();
  Sprite& getSprite(const std::string& name);

  void blitSprite(const std::string& name, int x, int y, size_t frame, Sprite::Dir dir, int xoff, int yoff, int wd, int ht, int alpha = 255);
  void blitColoredSprite(const std::string& name, int x, int y, size_t frame, Sprite::Dir dir, int xoff, int yoff, int wd, int ht, int alpha, int r, int g, int b, float angle = 0.0f);
  void blitSpriteMirrored(const std::string& name, int x, int y, size_t frame, Sprite::Dir dir, int xoff, int yoff, int wd, int ht, int alpha = 255);

  template<class T,class T2>
  void drawSprite(const std::string& name, T x, T2 y, size_t frame = 0, Sprite::Dir dir = Sprite::Dir::LEFT, int alpha = 255)
  {
    blitSprite(name, static_cast<int>(x), static_cast<int>(y), frame, dir, 0, 0, _sprites[name].width(), _sprites[name].height(), alpha);
  }

  template<class T,class T2>
  void drawColoredSprite(const std::string& name, T x, T2 y, size_t frame, int alpha, int r, int g, int b, float angle = 0.0f)
  {
    blitColoredSprite(name, static_cast<int>(x), static_cast<int>(y), frame, Sprite::Dir::LEFT, 0, 0, _sprites[name].width(), _sprites[name].height(), alpha, r, g, b, angle);
  }

  template<class T,class T2>
  void drawSpriteMirrored(const std::string& name, T x, T2 y, size_t frame = 0, Sprite::Dir dir = Sprite::Dir::LEFT)
  {
    blitSpriteMirrored(name, static_cast<int>(x), static_cast<int>(y), frame, dir, 0, 0, _sprites[name].width(), _sprites[name].height());
  }

private:
  std::map<std::string, Sprite> _sprites;
  std::map<std::string, Surface*> _sheets;

  void _offset_by_draw_points();
  void _expand_single_dir_sprites();

  void _loadSheetIfNeeded(std::string& sheet);
};
}; // namespace Graphics
}; // namespace Ironhead

#endif
