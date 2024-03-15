#include "Tileset.h"

#include "../ResourceManager.h"
#include "Surface.h"
#include "Renderer.h"

#include <cassert>
#include <cstdio>

namespace Ironhead
{
namespace Graphics
{

Tileset::Tileset() {}

Tileset::~Tileset()
{
  delete _tileset;
}

// load the given tileset into memory, replacing any other tileset.
bool Tileset::load(std::string& new_tileset)
{
  if (new_tileset != _current_tileset)
  {
    if (_tileset)
    {
      delete _tileset;
      _current_tileset = "";
    }

    _tileset = Surface::fromFile(ResourceManager::getInstance()->getLocalizedPath(new_tileset), true);
    if (!_tileset)
    {
      return false;
    }

    _current_tileset = new_tileset;
  }

  return true;
}

// draw the given tile from the current tileset to the screen
void Tileset::drawTile(int x, int y, int t)
{
  // 16 tiles per row on all tilesheet
  int srcx = (t % 16) * TILE_W;
  int srcy = (t / 16) * TILE_H;

  Renderer::getInstance()->drawSurface(_tileset, x, y, srcx, srcy, TILE_W, TILE_H);
}

void Tileset::reload()
{
  if (_current_tileset != "")
  {
    std::string tileset = _current_tileset;
    _current_tileset = "";
    load(tileset);
  }
}

}; //namespace Graphics
}; //namespace Ironhead