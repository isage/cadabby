#include "Sprites.h"

#include "../ResourceManager.h"
#include "Renderer.h"
#include "Surface.h"
#include "Sprite.h"
#include "../Utils/Logger.h"
#include "../common/misc.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <json.hpp>

namespace Ironhead
{
namespace Graphics
{

// offset things like blockl/r/u/d, bounding box etc by the draw point of all
// sprites so that these things are consistent with where the sprite appears to be
void Sprites::_offset_by_draw_points()
{
return;
/* TODO
  for (int s = 0; s < _num_sprites; s++)
  {
    int dx = -sprites[s].frame[0].dir[0].drawpoint.x;
    int dy = -sprites[s].frame[0].dir[0].drawpoint.y;

    sprites[s].bbox.offset(dx, dy);
    sprites[s].slopebox.offset(dx, dy);
    sprites[s].solidbox.offset(dx, dy);

    sprites[s].block_l.offset(dx, dy);
    sprites[s].block_r.offset(dx, dy);
    sprites[s].block_u.offset(dx, dy);
    sprites[s].block_d.offset(dx, dy);

    for (int f = 0; f < sprites[s].nframes; f++)
    {
      for (int d = 0; d < sprites[s].ndirs; d++)
      {
        int dx = -sprites[s].frame[f].dir[d].drawpoint.x;
        int dy = -sprites[s].frame[f].dir[d].drawpoint.y;
        sprites[s].frame[f].dir[d].pf_bbox.offset(dx, dy);
      }
    }
  }*/
}

// for sprites which only have 1 dir (no separate frames for left & right),
// create a 2nd identical dir as the rest of the engine doesn't bother
// with this complication.

void Sprites::_expand_single_dir_sprites()
{
return;
/* TODO
  for (int s = 0; s < _num_sprites; s++)
  {
    if (sprites[s].ndirs == 1)
    {
      sprites[s].ndirs = 2;
      for (int f = 0; f < sprites[s].nframes; f++)
        sprites[s].frame[f].dir[1] = sprites[s].frame[f].dir[0];
    }
    if (sprites[s].ndirs == 2)
    {
      sprites[s].ndirs = 4;
      for (int f = 0; f < sprites[s].nframes; f++)
      {
        sprites[s].frame[f].dir[2] = sprites[s].frame[f].dir[0];
        sprites[s].frame[f].dir[3] = sprites[s].frame[f].dir[0];
      }
    }
  }
  */
}

Sprites::Sprites() {}

Sprites::~Sprites()
{
  close();
}

bool Sprites::init()
{
  for(auto& p: std::filesystem::directory_iterator(ResourceManager::getInstance()->getPathForDir("sprites")))
  {
    LOG_DEBUG("Loading sprite: {}", p.path().string());
    std::ifstream fl;

    fl.open(widen(p.path()), std::ifstream::in | std::ifstream::binary);
    if (fl.is_open())
    {
        nlohmann::json json = nlohmann::json::parse(fl);
        Sprite sp = json;
        _sprites.emplace(sp.name(), std::move(sp));
        LOG_DEBUG("Sprite: {}, tileset: {}, frames: {}", sp.name(), sp.spritesheet(), sp.frames());
        fl.close();
    }
  }

// TODO
//  _offset_by_draw_points();
//  _expand_single_dir_sprites();

  return true;
}

void Sprites::close()
{
  flushSheets();
  _sheets.clear();
}

void Sprites::flushSheets()
{
  for (auto& [key, value]: _sheets)
  {
    delete value;
  }
}

void Sprites::_loadSheetIfNeeded(std::string& sheet)
{
  if (_sheets.find(sheet) == _sheets.end())
  {
    _sheets.emplace(sheet, Surface::fromFile(ResourceManager::getInstance()->getLocalizedPath("Pbm/"+sheet), true));
//    _sheets.at(sheet)->loadImage(ResourceManager::getInstance()->getLocalizedPath("Pbm/"+sheet), true);
  }
}

Sprite& Sprites::getSprite(const std::string& name)
{
    return _sprites.at(name);
}

// master sprite drawing function
void Sprites::blitSprite(const std::string& name, int x, int y, size_t frame, Sprite::Dir dir, int xoff, int yoff, int wd, int ht, int alpha)
{
  if (_sprites.find(name) != _sprites.end())
  {
    _loadSheetIfNeeded(_sprites.at(name).spritesheet());

    if (frame < _sprites.at(name).frames())
    {
      SpriteDir sprdir = _sprites.at(name).dir(frame, dir);

      _sheets.at(_sprites.at(name).spritesheet())->alpha = alpha;

      Renderer::getInstance()->drawSurface(_sheets.at(_sprites.at(name).spritesheet()), x, y, (sprdir.sheetOffset.x() + xoff),
                (sprdir.sheetOffset.y() + yoff), wd, ht);

      _sheets.at(_sprites.at(name).spritesheet())->alpha = 255;
    }
  }
}

void Sprites::blitColoredSprite(const std::string& name, int x, int y, size_t frame, Sprite::Dir dir, int xoff, int yoff, int wd, int ht, int alpha, int r, int b, int g, float angle)
{
  if (_sprites.find(name) != _sprites.end())
  {
    _loadSheetIfNeeded(_sprites.at(name).spritesheet());

    if (frame < _sprites.at(name).frames())
    {
      SpriteDir sprdir = _sprites.at(name).dir(frame, dir);

      _sheets.at(_sprites.at(name).spritesheet())->alpha = alpha;
      _sheets.at(_sprites.at(name).spritesheet())->r = r;
      _sheets.at(_sprites.at(name).spritesheet())->g = g;
      _sheets.at(_sprites.at(name).spritesheet())->b = b;

      Renderer::getInstance()->drawSurfaceEx(_sheets.at(_sprites.at(name).spritesheet()), x, y, (sprdir.sheetOffset.x() + xoff),
                (sprdir.sheetOffset.y() + yoff), wd, ht, angle);

      _sheets.at(_sprites.at(name).spritesheet())->alpha = 255;
      _sheets.at(_sprites.at(name).spritesheet())->r = 255;
      _sheets.at(_sprites.at(name).spritesheet())->g = 255;
      _sheets.at(_sprites.at(name).spritesheet())->b = 255;
    }
  }
}

void Sprites::blitSpriteMirrored(const std::string& name, int x, int y, size_t frame, Sprite::Dir dir, int xoff, int yoff, int wd, int ht, int alpha)
{
  if (_sprites.find(name) != _sprites.end())
  {
    _loadSheetIfNeeded(_sprites.at(name).spritesheet());

    if (frame < _sprites.at(name).frames())
    {
      SpriteDir sprdir = _sprites.at(name).dir(frame, dir);

      _sheets.at(_sprites.at(name).spritesheet())->alpha = alpha;

      Renderer::getInstance()->drawSurfaceMirrored(_sheets.at(_sprites.at(name).spritesheet()), x, y, (sprdir.sheetOffset.x() + xoff),
                (sprdir.sheetOffset.y() + yoff), wd, ht);

      _sheets.at(_sprites.at(name).spritesheet())->alpha = 255;
    }
  }
}

}; // namespace Graphics
}; // namespace Ironhead