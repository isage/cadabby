#ifndef GRAPHICS_RECT_H
#define GRAPHICS_RECT_H

#include "../Graphics/Point.h"
#include "../Graphics/Size.h"

#include <json.hpp>

using nlohmann::json;

namespace Ironhead
{
namespace Graphics
{

// static class for Rectangle-related function
class Rect
{
public:
    Rect() : _topLeft(0, 0), _size(0, 0) {}

    Rect(int x, int y, int width, int height);

    int x() const;
    int y() const;

    void setX(int x);
    void setY(int y);

    int width() const;
    int height() const;

    void setWidth(int width);
    void setHeight(int height);


    // checks if given point is located withing the rectangle specified by size
    static bool inRect(const Point& needle, const Size& size);

    // checks if given point is located withing the rectangle with given top-left position and size
    static bool inRect(const Point& needle, const Point& topLeft, const Size& size);

    // checks if two rectangles, given as their top-left positions and sizes, intersect each other
    static bool intersects(const Point& topLeft1, const Size& size1, const Point& topLeft2, const Size& size2);
private:
    Point _topLeft;
    Size _size;
public:
    friend void from_json(const json& j, Rect& p)
    {
        p._topLeft.setX(j.at("x").get<int>());
        p._topLeft.setY(j.at("y").get<int>());
        p._size.setWidth(j.at("width").get<int>());
        p._size.setHeight(j.at("height").get<int>());
    }

    friend void to_json(json& j, const Rect& p)
    {
        j = json{
            {"x", p._topLeft.x()},
            {"y", p._topLeft.y()},
            {"width", p._size.width()},
            {"height", p._size.height()}
        };
    }

};

}
}
#endif // GRAPHICS_RECT_H
