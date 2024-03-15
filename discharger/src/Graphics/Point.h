#ifndef GRAPHICS_POINT_H
#define GRAPHICS_POINT_H

#include <json.hpp>

using nlohmann::json;

namespace Ironhead
{
namespace Graphics
{

/**
 * Represents a Point on screen: x and y coordinates, in pixels.
 */
class Point
{
public:
    Point() : _x(0), _y(0) {}

    Point(int x, int y) : _x(x), _y(y) {}

    int x() const;
    int y() const;

    // Reference to x coordinate
    int& rx();
    // Reference to y coordinate
    int& ry();

    void setX(int x);
    void setY(int y);

    Point& operator +=(const Point& rhs);
    Point& operator -=(const Point& rhs);
    Point& operator *=(double rhs);
    Point& operator /=(double rhs);

    bool operator ==(const Point& rhs) const;
    bool operator !=(const Point& rhs) const;

    friend Point operator +(Point lhs, const Point& rhs);
    friend Point operator -(Point lhs, const Point& rhs);
    friend Point operator *(Point lhs, double rhs);
    friend Point operator /(Point lhs, double rhs);

    // Addition of given Point
    Point add(const Point& rhs) const;
    // Subtraction of given Point
    Point sub(const Point& rhs) const;
    // Multiplication by given number
    Point mul(double rhs) const;
    // Division by given number
    Point div(double rhs) const;

protected:
    int _x;
    int _y;
public:
    friend void from_json(const json& j, Point& p)
    {
        j.at("x").get_to(p._x);
        j.at("y").get_to(p._y);
    }

    friend void to_json(json& j, const Point& p)
    {
        j = json{{"x", p._x}, {"y", p._y}};
    }
};

}
}
#endif //GRAPHICS_POINT_H
