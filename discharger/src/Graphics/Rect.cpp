#include "../Graphics/Rect.h"

#include <algorithm>

namespace Ironhead
{
namespace Graphics
{

Rect::Rect(int x, int y, int width, int height)
{
    setX(x);
    setY(y);
    setWidth(width);
    setHeight(height);
}

int Rect::x() const
{
    return _topLeft.x();
}

int Rect::y() const
{
    return _topLeft.y();
}

void Rect::setX(int x)
{
    _topLeft.setX(x);
}

void Rect::setY(int y)
{
    _topLeft.setY(y);
}


int Rect::width() const
{
    return _size.width();
}

int Rect::height() const
{
    return _size.height();
}

void Rect::setWidth(int width)
{
    if (width < 0)
    {
        width = 0;
    }
    _size.setWidth(width);
}

void Rect::setHeight(int height)
{
    if (height < 0)
    {
        height = 0;
    }
    _size.setHeight(height);
}

bool Rect::inRect(const Point& needle, const Size& size)
{
    return (needle.x() >= 0
            && needle.x() < size.width()
            && needle.y() >= 0
            && needle.y() < size.height());
}

bool Rect::inRect(const Point& needle, const Point& topLeft, const Size& size)
{
    Point bottomRight = topLeft + size;
    return (needle.x() >= topLeft.x()
            && needle.x() < bottomRight.x()
            && needle.y() >= topLeft.y()
            && needle.y() < bottomRight.y());
}

bool Rect::intersects(const Point& topLeft1, const Size& size1, const Point& topLeft2, const Size& size2)
{
    const Point bottomRight1 = topLeft1 + size1;
    const Point bottomRight2 = topLeft2 + size2;

    return !(std::max(topLeft1.x(), topLeft2.x()) > std::min(bottomRight1.x(), bottomRight2.x())
             || std::max(topLeft1.y(), topLeft2.y()) > std::min(bottomRight1.y(), bottomRight2.y()));
}

}
}
