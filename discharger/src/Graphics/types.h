#ifndef _G_TYPES_H
#define _G_TYPES_H

#include <cstdint>
#include <SDL.h>

namespace Ironhead
{
namespace Graphics
{

struct NXColor
{
  uint8_t r, g, b;

  NXColor()
      : r(0)
      , g(0)
      , b(0)
  {
  }

  NXColor(uint8_t rr, uint8_t gg, uint8_t bb)
      : r(rr)
      , g(gg)
      , b(bb)
  {
  }

  NXColor(uint32_t hexcolor)
  {
    r = hexcolor >> 16;
    g = hexcolor >> 8;
    b = hexcolor;
  }

  inline bool operator==(const NXColor& rhs) const
  {
    return (this->r == rhs.r && this->g == rhs.g && this->b == rhs.b);
  }

  inline bool operator!=(const NXColor& rhs) const
  {
    return (this->r != rhs.r || this->g != rhs.g || this->b != rhs.b);
  }
};

}
}
#endif
