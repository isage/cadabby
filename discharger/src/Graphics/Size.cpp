#include "../Graphics/Size.h"

namespace Ironhead
{
namespace Graphics
{

Size::Size(int width, int height)
{
    setWidth(width);
    setHeight(height);
}

Size::Size(const Point& rhs)
{
    *this = rhs;
}

Size& Size::operator =(const Point& rhs)
{
    setWidth(rhs.x());
    setHeight(rhs.y());
    return *this;
}

Size::operator Point() const
{
    return Point(_width, _height);
}

int Size::width() const
{
    return _width;
}

int Size::height() const
{
    return _height;
}

/*int& Size::rwidth()
{
    return _width;
}

int& Size::rheight()
{
    return _height;
}*/

void Size::setWidth(int width)
{
    if (width < 0)
    {
        width = 0;
    }
    _width = width;
}

void Size::setHeight(int height)
{
    if (height < 0)
    {
        height = 0;
    }
    _height = height;
}

Size& Size::operator +=(const Size& rhs)
{
    setWidth(_width + rhs._width);
    setHeight(_height + rhs._height);
    return *this;
}

Size& Size::operator -=(const Size& rhs)
{
    setWidth(_width - rhs._width);
    setHeight(_height - rhs._height);
    return *this;
}

Size& Size::operator *=(double rhs)
{
    setWidth((int)(_width * rhs));
    setHeight((int)(_height * rhs));
    return *this;
}

Size& Size::operator /=(double rhs)
{
    setWidth((int)(_width / rhs));
    setHeight((int)(_height / rhs));
    return *this;
}

bool Size::operator ==(const Size& rhs) const
{
    return _width == rhs._width && _height == rhs._height;
}

bool Size::operator !=(const Size& rhs) const
{
    return !(*this == rhs);
}

Size operator +(Size lhs, const Size& rhs)
{
    lhs += rhs;
    return lhs;
}

Size operator -(Size lhs, const Size& rhs)
{
    lhs -= rhs;
    return lhs;
}

Size operator *(Size lhs, double rhs)
{
    lhs *= rhs;
    return lhs;
}

Size operator /(Size lhs, double rhs)
{
    lhs /= rhs;
    return lhs;
}

Size Size::add(const Size& rhs) const
{
    return *this + rhs;
}

Size Size::sub(const Size& rhs) const
{
    return *this - rhs;
}

Size Size::mul(double rhs) const
{
    return *this * rhs;
}

Size Size::div(double rhs) const
{
    return *this / rhs;
}

}
}
