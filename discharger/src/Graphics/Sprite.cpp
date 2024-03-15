#include "Sprite.h"

#include "../Graphics/Point.h"
#include "../Graphics/Rect.h"

namespace Ironhead
{

namespace Graphics
{

  Sprite::Sprite() {}
  Sprite::~Sprite(){}

  SpriteDir& Sprite::dir(int frame, Dir dir)
  {
    int _dir = (int)dir;
    _dir %= _ndirs;
    return _frames.at(frame).dirs[_dir];
  }

  int Sprite::width()
  {
    return _size.width();
  }

  int Sprite::height()
  {
    return _size.height();
  }

  size_t Sprite::frames()
  {
    return _frames.size();
  }

  std::string& Sprite::spritesheet()
  {
    return _spritesheet;
  }

  std::string& Sprite::name()
  {
    return _name;
  }
}
}
