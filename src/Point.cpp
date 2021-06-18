//
// Initial class given by professor Marcio Sarroglia Pinho (PUCRS)
// Modified by Eduardo Andrade and Marcelo Heredia
//

#include "../headers/Point.h"

Point::Point()
{
    x = y = z = 0;
}

Point::Point(GLfloat x, GLfloat y, GLfloat z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Point::print() const
{
    cout << "(" << x << ", " << y << ")" << endl << flush;
}

bool Point::operator==(const Point &other) const
{
    return (x == other.x) && (y == other.y);
}

bool Point::operator<=(const Point &other) const
{
    return (x <= other.x) && (y <= other.y);
}

bool Point::operator>=(const Point &other) const
{
    return (x >= other.x) && (y >= other.y);
}

Point &Point::operator+=(const Point &other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}

Point &Point::operator-=(const Point &other)
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

Point operator*(const Point &p1, const Point &p2)
{
    Point res;
    res.x = p1.x * p2.x;
    res.y = p2.y * p2.y;
    return res;
}

Point operator*(const Point &p, const double d)
{
    Point res;
    res.x = p.x * (GLfloat) d;
    res.y = p.y * (GLfloat) d;
    return res;
}

Point operator+(const Point &p1, const Point &p2)
{
    Point res;
    res.x = p1.x + p2.x;
    res.y = p1.y + p2.y;
    return res;
}

Point operator-(const Point &p1, const Point &p2)
{
    Point res;
    res.x = p1.x - p2.x;
    res.y = p1.y - p2.y;
    return res;
}

Point get_max(Point p1, Point p2)
{
    Point max;

    max.x = (p2.x > p1.x) ? p2.x : p1.x;
    max.y = (p2.y > p1.y) ? p2.y : p1.y;
    max.z = (p2.z > p1.x) ? p2.z : p1.z;
    return max;
}

Point get_min(Point p1, Point p2)
{
    Point min;

    min.x = (p2.x < p1.x) ? p2.x : p1.x;
    min.y = (p2.y < p1.y) ? p2.y : p1.y;
    min.z = (p2.z < p1.x) ? p2.z : p1.z;

    return min;
}

/**
 * Calcula o produto vetorial entre os vetores V1 e V2 e guarda em x,y,z de vResult
 */
void prod_vetorial(Point v1, Point v2, Point &vResult)
{
    vResult.x = v1.y * v2.z - (v1.z * v2.y);
    vResult.y = v1.z * v2.x - (v1.x * v2.z);
    vResult.z = v1.x * v2.y - (v1.y * v2.x);
}
