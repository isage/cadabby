#ifndef GRAPHICS_SPRITE_H
#define GRAPHICS_SPRITE_H


#include "../Graphics/Point.h"
#include "../Graphics/Rect.h"
#include "../Graphics/Size.h"

#include <json.hpp>
#include <vector>
#include <cstdint>


// a sprite has certain settings global to the whole sprite.
// each sprite also contains a certain number of frames.
// each frame contains one or more directions, for example for when that
// frame is facing left or right, and sometimes up or down.
// the number of directions is settable per-sprite, but each frame in
// a given sprite always has the same number of directions.

#define SIF_MAX_DIRS 4

using nlohmann::json;

namespace Ironhead
{

namespace Graphics
{

struct SpriteDir
{
  // the offset on the sprite sheet where this frame is located.
  Point sheetOffset;

  // when a sprite is drawn, it's sprite will be offset such that this pixel is
  // placed at the (x,y) coordinates that were given for the sprite to be drawn at.
  // i.e., it's a "hot point".
  // this also offsets the bounding boxes and blockl/r/u/d points but not the
  // action points or spawn point.
  // it is not super-well tested and isn't intended to be frequently used on objects which
  // interact with the world or are solid, etc. mostly it is for use with objects
  // such as the caret effects etc, so that their o->x & o->y can represent their center
  // point instead of their upper-left corner.
  Point drawPoint;
};

struct SpriteFrame
{
  SpriteDir dirs[SIF_MAX_DIRS];
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpriteDir, sheetOffset, drawPoint)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpriteFrame, dirs)

class Sprite
{
public:
  enum class Dir
  {
    LEFT = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 3
  };

  Sprite();
  ~Sprite();

  SpriteDir& dir(int frame, Dir dir);
  int width();
  int height();
  std::string& spritesheet();
  std::string& name();
  size_t frames();

private:
  Size _size;

  std::string _name;
  std::string _spritesheet;

  int _ndirs;

  std::vector<SpriteFrame> _frames;

  Rect _bbox;     // bounding box for hit detection with other sprites
  Rect _solidbox; // bounding box for solidity w/ other objects

  // when an object is spawned or changed from one type to another, this point is subtracted
  // from it's x,y coordinates, so that this point ends up being centered on the given x,y coords.
  Point _spawnPoint;

  // points to check for setting the corresponding block vars on the object
  // (which determine whether it's blocked on that side by a solid wall or object).
  std::vector<Point> _block_l;
  std::vector<Point> _block_r;
  std::vector<Point> _block_u;
  std::vector<Point> _block_d;

public:
  friend void from_json(const json& j, Sprite& s) {
    j["sheet"].get_to(s._spritesheet);
    j["name"].get_to(s._name);

    // TODO: get rid of this shit
    j["ndirs"].get_to(s._ndirs);

    j["block_d"].get_to(s._block_d);
    j["block_u"].get_to(s._block_u);
    j["block_l"].get_to(s._block_l);
    j["block_r"].get_to(s._block_r);

    j["frames"].get_to(s._frames);

    s._size.setWidth(j["width"].get<int>());
    s._size.setHeight(j["height"].get<int>());

    j["bbox"].get_to(s._bbox);
    j["solidbox"].get_to(s._solidbox);
    j["spawnpoint"].get_to(s._spawnPoint);
  }


};



}
}

#endif
