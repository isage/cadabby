#ifndef GRAPHICS_SIZE_H
#define GRAPHICS_SIZE_H

#include "../Graphics/Point.h"

#include <json.hpp>

using nlohmann::json;


namespace Ironhead
{
namespace Graphics
{

/**
 * Represents a Size of a rectangular object: width and height, in pixels.
 */
class Size
{
public:
    Size() : _width(0), _height(0) {}

    Size(int width, int height);

    explicit Size(const Point& rhs);

    Size& operator=(const Point& rhs);

    // Implicit conversion to Point
    operator Point() const;

    int width() const;
    int height() const;

    /*int& rwidth();
    int& rheight();*/

    void setWidth(int width);
    void setHeight(int height);

    Size& operator +=(const Size& rhs);
    Size& operator -=(const Size& rhs);
    Size& operator *=(double rhs);
    Size& operator /=(double rhs);

    bool operator ==(const Size& rhs) const;
    bool operator !=(const Size& rhs) const;

    friend Size operator +(Size lhs, const Size& rhs);
    friend Size operator -(Size lhs, const Size& rhs);
    friend Size operator *(Size lhs, double rhs);
    friend Size operator /(Size lhs, double rhs);

    // Addition of another Size
    Size add(const Size& rhs) const;
    // Subtraction of another Size
    Size sub(const Size& rhs) const;
    // Multiplication of width/height by given number
    Size mul(double rhs) const;
    // Division by given number
    Size div(double rhs) const;

protected:
    int _width;
    int _height;
public:
    friend void from_json(const json& j, Size& p)
    {
        j.at("width").get_to(p._width);
        j.at("height").get_to(p._height);
    }

    friend void to_json(json& j, const Size& p)
    {
        j = json{{"width", p._width}, {"height", p._height}};
    }

};

}
}
#endif // GRAPHICS_SIZE_H
