#ifndef _RENDERER_H
#define _RENDERER_H

#include "../Base/Singleton.h"
//#include "../object.h"
#include "types.h"
#include "Surface.h"
#include "Font.h"
#include "Tileset.h"
#include "Sprites.h"

#include <SDL.h>


namespace Ironhead
{
namespace Graphics
{

const NXColor COLOR_DK_BLUE(0,0,0x21);
const NXColor COLOR_BLACK(0,0,0);
const NXColor COLOR_WHITE(0xFF,0xFF,0xFF);
const NXColor COLOR_GREEN1(0x0,0xFF,0x0);
const NXColor COLOR_GREEN2(0x0,0x98,0x0);
const NXColor COLOR_GREEN3(0x0,0x4E,0x0);
const NXColor COLOR_GREEN4(0x0,0x19,0x0);

typedef struct
{
  char *name;
  uint32_t width;
  uint32_t height;
  uint32_t base_width;
  uint32_t base_height;
  uint32_t scale;
  bool widescreen;
  bool enabled;
} gres_t;

class Renderer
{
  public:
    static Renderer *getInstance();

    int screenWidth = 320;
    int screenHeight = 240;
    bool widescreen = false;
    int scale = 1;

    bool init(int resolution);
    void close();

    bool isWindowVisible();

    bool initVideo();
    void setFullscreen(bool enable);

    bool setResolution(int factor, bool restoreOnFailure = true);
    const gres_t *getResolutions(bool full_list = false);
    int getResolutionCount();

    bool flushAll();

    void showLoadingScreen();

    void drawSurface(Surface *src, int x, int y);
    void drawSurface(Surface *src, int dstx, int dsty, int srcx, int srcy, int wd, int ht);
    void drawSurfaceEx(Surface *src, int dstx, int dsty, int srcx, int srcy, int wd, int ht, float angle = 0.0f);
    void drawSurfaceMirrored(Surface *src, int dstx, int dsty, int srcx, int srcy, int wd, int ht);

    void blitPatternAcross(Surface *sfc, int x_dst, int y_dst, int y_src, int height);

    void clearScreen(NXColor color);
    void clearScreen(uint8_t r, uint8_t g, uint8_t b);

    void drawLine(int x1, int y1, int x2, int y2, NXColor color);
    void drawLine(int x1, int y1, int x2, int y2, uint8_t r,uint8_t g,uint8_t b);

    void drawRect(int x1, int y1, int x2, int y2, NXColor color);

    void drawRect(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a = SDL_ALPHA_OPAQUE);
    void drawRect(SDL_Rect *rect, uint8_t r, uint8_t g, uint8_t b);
    void drawRect(SDL_Rect *rect, NXColor color);

    void fillRect(int x1, int y1, int x2, int y2, NXColor color);
    void fillRect(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a = SDL_ALPHA_OPAQUE);
    void fillRect(SDL_Rect *rect, uint8_t r, uint8_t g, uint8_t b);
    void fillRect(SDL_Rect *rect, NXColor color);

    void drawPixel(int x, int y, NXColor color);
    void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);

    void setClip(int x, int y, int w, int h);
    void setClip(SDL_Rect *rect);
    void clearClip();
    bool isClipSet();
    void clip(SDL_Rect &srcrect, SDL_Rect &dstrect);
    void clipScaled(SDL_Rect &srcrect, SDL_Rect &dstrect);

    void saveScreenshot();

    void tintScreen();
    void flip();
    SDL_Renderer* renderer();
    SDL_Window* window();
    Font font;
    Tileset tileset;
    Sprites sprites;

  private:
    SDL_Window *_window = nullptr;
    SDL_Renderer *_renderer = nullptr;
    int _current_res = -1;
    bool _need_clip = false;
    SDL_Rect _clip_rect;
    SDL_Texture* _spot_light;

  protected:
    friend class Base::Singleton<Renderer>;

    Renderer();
    ~Renderer();
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
};

void inline Renderer::drawSurface(Surface *src, int dstx, int dsty)
{
  drawSurface(src, dstx, dsty, 0, 0, src->width(), src->height());
}

void inline Renderer::drawRect(int x1, int y1, int x2, int y2, NXColor color)
{
  drawRect(x1, y1, x2, y2, color.r, color.g, color.b, 255);
}

void inline Renderer::drawRect(SDL_Rect *rect, uint8_t r, uint8_t g, uint8_t b)
{
  drawRect(rect->x, rect->y, rect->x + (rect->w - 1), rect->y + (rect->h - 1), r, g, b, 255);
}

void inline Renderer::drawRect(SDL_Rect *rect, NXColor color)
{
  drawRect(rect->x, rect->y, rect->x + (rect->w - 1), rect->y + (rect->h - 1), color.r, color.g, color.b,  255);
}

void inline Renderer::fillRect(int x1, int y1, int x2, int y2, NXColor color)
{
  fillRect(x1, y1, x2, y2, color.r, color.g, color.b,  255);
}

void inline Renderer::fillRect(SDL_Rect *rect, uint8_t r, uint8_t g, uint8_t b)
{
  fillRect(rect->x, rect->y, rect->x + (rect->w - 1), rect->y + (rect->h - 1), r, g, b,  255);
}

void inline Renderer::fillRect(SDL_Rect *rect, NXColor color)
{
  fillRect(rect->x, rect->y, rect->x + (rect->w - 1), rect->y + (rect->h - 1), color.r, color.g, color.b,  255);
}

void inline Renderer::drawPixel(int x, int y, NXColor color)
{
  drawPixel(x, y, color.r, color.g, color.b);
}

void inline Renderer::clearScreen(NXColor color)
{
  clearScreen(color.r, color.g, color.b);
}

void inline Renderer::setClip(SDL_Rect *rect)
{
  setClip(rect->x, rect->y, rect->w, rect->h);
}



}; // namespace Graphics
}; // namespace Ironhead
#endif
