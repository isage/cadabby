#ifndef _TILESET_H
#define _TILESET_H

#include "Surface.h"

#define TILE_W 16
#define TILE_H 16

namespace Ironhead
{
namespace Graphics
{

class Tileset
{
  public:
    Tileset();
    ~Tileset();
    bool init();
    void close();

    bool load(std::string& new_tileset);
    void reload();

    void drawTile(int x, int y, int t);
  private:
    Surface *_tileset = nullptr;
    std::string _current_tileset = "";
};

}; // namespace Graphics
}; // namespace Tileset

#endif
